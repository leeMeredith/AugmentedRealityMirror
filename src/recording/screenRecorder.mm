#include "screenRecorder.h"

#import <AVFoundation/AVFoundation.h>
#import <CoreVideo/CoreVideo.h>

#include <algorithm>
#include <cmath>

struct screenRecorder::implementation {
    AVAssetWriter* writer = nil;
    AVAssetWriterInput* input = nil;
    AVAssetWriterInputPixelBufferAdaptor* adaptor = nil;
    int width = 0;
    int height = 0;
    float frameRate = 30.0f;
    bool recording = false;
    std::string error;
    std::string path;

    void clearWriter() {
        adaptor = nil;
        input = nil;
        writer = nil;
    }

    void setError(const std::string& prefix, NSError* nativeError = nil) {
        error = prefix;
        if (nativeError != nil) {
            const char* description = nativeError.localizedDescription.UTF8String;
            if (description != nullptr) {
                error += ": ";
                error += description;
            }
        }
    }
};

screenRecorder::screenRecorder()
: impl(std::make_unique<implementation>()) {
}

screenRecorder::~screenRecorder() {
    if (impl->recording) {
        stop();
    }
}

bool screenRecorder::start(
    const std::string& requestedOutputPath,
    int requestedWidth,
    int requestedHeight,
    float requestedFrameRate) {
    @autoreleasepool {
        impl->error.clear();
        if (impl->recording) {
            impl->error = "A recording is already active";
            return false;
        }
        if (requestedWidth <= 0 || requestedHeight <= 0 || requestedFrameRate <= 0.0f) {
            impl->error = "Invalid recording dimensions or frame rate";
            return false;
        }

        NSString* nativePath = [NSString stringWithUTF8String:requestedOutputPath.c_str()];
        if (nativePath == nil) {
            impl->error = "Invalid recording path";
            return false;
        }

        NSError* writerError = nil;
        NSURL* outputUrl = [NSURL fileURLWithPath:nativePath];
        impl->writer = [[AVAssetWriter alloc]
            initWithURL:outputUrl
            fileType:AVFileTypeQuickTimeMovie
            error:&writerError];
        if (impl->writer == nil) {
            impl->setError("Could not create video writer", writerError);
            impl->clearWriter();
            return false;
        }

        NSDictionary* compression = @{
            AVVideoAverageBitRateKey: @(requestedWidth * requestedHeight * 8)
        };
        NSDictionary* videoSettings = @{
            AVVideoCodecKey: AVVideoCodecTypeH264,
            AVVideoWidthKey: @(requestedWidth),
            AVVideoHeightKey: @(requestedHeight),
            AVVideoCompressionPropertiesKey: compression
        };

        impl->input = [AVAssetWriterInput
            assetWriterInputWithMediaType:AVMediaTypeVideo
            outputSettings:videoSettings];
        impl->input.expectsMediaDataInRealTime = YES;

        NSDictionary* pixelAttributes = @{
            (NSString*)kCVPixelBufferPixelFormatTypeKey: @(kCVPixelFormatType_32BGRA),
            (NSString*)kCVPixelBufferWidthKey: @(requestedWidth),
            (NSString*)kCVPixelBufferHeightKey: @(requestedHeight),
            (NSString*)kCVPixelBufferIOSurfacePropertiesKey: @{}
        };
        impl->adaptor = [AVAssetWriterInputPixelBufferAdaptor
            assetWriterInputPixelBufferAdaptorWithAssetWriterInput:impl->input
            sourcePixelBufferAttributes:pixelAttributes];

        if (![impl->writer canAddInput:impl->input]) {
            impl->error = "Could not add the video input";
            impl->clearWriter();
            return false;
        }
        [impl->writer addInput:impl->input];

        if (![impl->writer startWriting]) {
            impl->setError("Could not start video writing", impl->writer.error);
            impl->clearWriter();
            return false;
        }
        [impl->writer startSessionAtSourceTime:kCMTimeZero];

        impl->width = requestedWidth;
        impl->height = requestedHeight;
        impl->frameRate = requestedFrameRate;
        impl->path = requestedOutputPath;
        impl->recording = true;
        return true;
    }
}

bool screenRecorder::addFrame(const ofPixels& pixels, double elapsedSeconds) {
    @autoreleasepool {
        impl->error.clear();
        if (!impl->recording) {
            impl->error = "No recording is active";
            return false;
        }
        if (pixels.getWidth() != impl->width || pixels.getHeight() != impl->height) {
            impl->error = "Frame size changed during recording";
            return false;
        }
        if (pixels.getNumChannels() < 3) {
            impl->error = "Recording frames must contain RGB pixels";
            return false;
        }
        if (!impl->input.readyForMoreMediaData) {
            return true;
        }

        CVPixelBufferRef pixelBuffer = nullptr;
        const CVReturn createResult = CVPixelBufferPoolCreatePixelBuffer(
            kCFAllocatorDefault,
            impl->adaptor.pixelBufferPool,
            &pixelBuffer);
        if (createResult != kCVReturnSuccess || pixelBuffer == nullptr) {
            impl->error = "Could not allocate a video frame";
            return false;
        }

        CVPixelBufferLockBaseAddress(pixelBuffer, 0);
        auto* destination = static_cast<unsigned char*>(CVPixelBufferGetBaseAddress(pixelBuffer));
        const std::size_t destinationStride = CVPixelBufferGetBytesPerRow(pixelBuffer);
        const unsigned char* source = pixels.getData();
        const std::size_t sourceChannels = pixels.getNumChannels();
        const std::size_t sourceStride = pixels.getWidth() * sourceChannels;

        for (int y = 0; y < impl->height; ++y) {
            const unsigned char* sourceRow = source + y * sourceStride;
            unsigned char* destinationRow = destination + y * destinationStride;
            for (int x = 0; x < impl->width; ++x) {
                const unsigned char* sourcePixel = sourceRow + x * sourceChannels;
                unsigned char* destinationPixel = destinationRow + x * 4;
                destinationPixel[0] = sourcePixel[2];
                destinationPixel[1] = sourcePixel[1];
                destinationPixel[2] = sourcePixel[0];
                destinationPixel[3] = sourceChannels >= 4 ? sourcePixel[3] : 255;
            }
        }
        CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);

        const CMTime presentationTime = CMTimeMakeWithSeconds(
            std::max(0.0, elapsedSeconds),
            600);
        const bool appended = [impl->adaptor
            appendPixelBuffer:pixelBuffer
            withPresentationTime:presentationTime];
        CVPixelBufferRelease(pixelBuffer);

        if (!appended) {
            impl->setError("Could not append a video frame", impl->writer.error);
            return false;
        }
        return true;
    }
}

bool screenRecorder::stop() {
    @autoreleasepool {
        impl->error.clear();
        if (!impl->recording) {
            return true;
        }

        [impl->input markAsFinished];
        dispatch_semaphore_t finished = dispatch_semaphore_create(0);
        [impl->writer finishWritingWithCompletionHandler:^{
            dispatch_semaphore_signal(finished);
        }];

        const auto timeout = dispatch_time(DISPATCH_TIME_NOW, 10 * NSEC_PER_SEC);
        const long waitResult = dispatch_semaphore_wait(finished, timeout);
        const AVAssetWriterStatus status = impl->writer.status;
        NSError* writerError = impl->writer.error;

        impl->recording = false;
        impl->clearWriter();

        if (waitResult != 0) {
            impl->error = "Timed out while finishing the video";
            return false;
        }
        if (status != AVAssetWriterStatusCompleted) {
            impl->setError("Video did not finish correctly", writerError);
            return false;
        }
        return true;
    }
}

bool screenRecorder::isRecording() const {
    return impl->recording;
}

const std::string& screenRecorder::lastError() const {
    return impl->error;
}

const std::string& screenRecorder::outputPath() const {
    return impl->path;
}
