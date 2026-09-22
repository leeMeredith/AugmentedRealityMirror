#include "../src/gui/ArmJsonDocument.h"

#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>

int main() {
    namespace fs = std::filesystem;
    const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
    const fs::path path = fs::temp_directory_path() /
                          ("arm-json-document-" + std::to_string(stamp) + ".json");

    ArmJsonDocument document;
    assert(document.addChild("FLYTHROUGH"));
    assert(document.setTo("FLYTHROUGH"));

    ArmJsonDocument firstRecord;
    assert(firstRecord.addChild("RECORD"));
    assert(firstRecord.setTo("RECORD"));
    assert(firstRecord.addValue("PARTICIPANT", "synthetic"));
    assert(firstRecord.addValue("CAMERA", 0));
    assert(firstRecord.addValue("CAMERA", 1));
    assert(firstRecord.addValue("POS_X", 12.5f));
    assert(firstRecord.addValue("POS_X", -4.25f));
    assert(document.addDocument(firstRecord));

    ArmJsonDocument secondRecord;
    assert(secondRecord.addChild("RECORD"));
    assert(secondRecord.setTo("RECORD"));
    assert(secondRecord.addValue("PARTICIPANT", "another synthetic"));
    assert(document.addDocument(secondRecord));
    assert(document.getNumChildren("RECORD") == 2);
    assert(document.save(path));

    ArmJsonDocument loaded;
    assert(loaded.load(path));
    assert(loaded.getNumChildren("RECORD") == 2);
    assert(loaded.setTo("RECORD[0]"));
    assert(loaded.getNumChildren("CAMERA") == 2);
    assert(loaded.getValue<int>("CAMERA[1]") == 1);
    assert(loaded.getValue<float>("POS_X[1]") == -4.25f);
    assert(loaded.getValue<std::string>("PARTICIPANT") == "synthetic");
    assert(loaded.getValue<int>("MISSING", 7) == 7);
    assert(loaded.setValue("PARTICIPANT", std::string("edited")));
    assert(loaded.remove("CAMERA[0]"));
    assert(loaded.getValue<int>("CAMERA[0]") == 1);
    assert(loaded.setTo("RECORD[1]"));
    assert(loaded.getValue<std::string>("PARTICIPANT") == "another synthetic");
    assert(loaded.setToParent());
    assert(loaded.save(path));

    ArmJsonDocument reloaded;
    assert(reloaded.load(path));
    assert(reloaded.setTo("RECORD"));
    assert(reloaded.getValue<std::string>("PARTICIPANT") == "edited");
    fs::remove(path);

    assert(armJsonRecordPath("DemoMe.xml", false) ==
           "record/address/flythrough/DemoMe.json");
    assert(armJsonRecordPath("DemoMeSessions.json", true) ==
           "record/address/session/DemoMeSessions.json");

    {
        std::ofstream broken(path);
        broken << "{broken json";
    }
    assert(!reloaded.load(path));
    assert(!reloaded.save(path));
    {
        std::ifstream preserved(path);
        std::string contents;
        std::getline(preserved, contents);
        assert(contents == "{broken json");
    }
    fs::remove(path);
    std::cout << "ARM JSON document roundtrip passed\n";
}
