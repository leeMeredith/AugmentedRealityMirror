#pragma once

#include <nlohmann/json.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

inline std::string armJsonRecordPath(const std::string& input, bool session) {
    std::string name = std::filesystem::path(input).filename().string();
    if (name.ends_with(".xml") || name.ends_with(".json"))
        name.erase(name.find_last_of('.'));
    if (session && !name.ends_with("Sessions")) name += "Sessions";
    return std::string("record/address/") + (session ? "session/" : "flythrough/") +
           name + ".json";
}

// JSON storage for ARM's repeated record fields. The small path API keeps the
// existing recording controls intact while the on-disk format changes.
class ArmJsonDocument {
public:
    using Json = nlohmann::json;

    bool load(const std::filesystem::path& path) {
        clear();
        std::ifstream input(path);
        if (!input) {
            std::error_code error;
            if (std::filesystem::exists(path, error)) unreadablePath_ = path;
            return false;
        }
        try {
            input >> document_;
            if (!document_.is_object() || document_.size() != 1 ||
                !document_.begin().value().is_object()) {
                clear();
                unreadablePath_ = path;
                return false;
            }
            cursor_.push_back({document_.begin().key(), 0});
            return true;
        } catch (const std::exception&) {
            clear();
            unreadablePath_ = path;
            return false;
        }
    }

    bool save(const std::filesystem::path& path) const {
        if (unreadablePath_ == path) {
            std::cerr << "Refusing to replace unreadable JSON record: " << path << '\n';
            return false;
        }
        std::ofstream output(path, std::ios::trunc);
        if (!output) return false;
        output << document_.dump(2) << '\n';
        return static_cast<bool>(output);
    }

    void clear() {
        document_ = Json::object();
        cursor_.clear();
        unreadablePath_.reset();
    }

    bool addChild(const std::string& name) {
        return append(name, Json::object());
    }

    bool addDocument(const ArmJsonDocument& other) {
        if (!other.document_.is_object() || other.document_.size() != 1) return false;
        return append(other.document_.begin().key(), other.document_.begin().value());
    }

    template <typename T>
    bool addValue(const std::string& name, const T& value) {
        return append(name, Json(value));
    }

    bool setAttribute(const std::string& name, const std::string& value) {
        Json* current = atCursor();
        if (!current || !current->is_object()) return false;
        (*current)[name] = value;
        return true;
    }

    bool setTo(const std::string& path) {
        const Segment segment = parse(path);
        if (!cursor_.empty() && cursor_.back().name == segment.name &&
            cursor_.back().index == segment.index) return true;
        if (cursor_.size() > 1 && cursor_.back().name == segment.name) {
            const Segment previous = cursor_.back();
            cursor_.pop_back();
            if (find(atCursor(), segment)) {
                cursor_.push_back(segment);
                return true;
            }
            cursor_.push_back(previous);
            return false;
        }
        const Json* current = atCursor();
        if (!find(current, segment)) return false;
        cursor_.push_back(segment);
        return true;
    }

    bool setToParent() {
        if (cursor_.size() <= 1) return false;
        cursor_.pop_back();
        return true;
    }

    bool exists(const std::string& path) const {
        const Segment segment = parse(path);
        if (!cursor_.empty() && cursor_.back().name == segment.name &&
            cursor_.back().index == segment.index) return true;
        return find(atCursor(), segment) != nullptr;
    }

    int getNumChildren(const std::string& name) const {
        const Json* current = atCursor();
        if (!current || !current->is_object()) return 0;
        auto it = current->find(name);
        if (it == current->end()) return 0;
        return static_cast<int>(it->is_array() ? it->size() : 1);
    }

    template <typename T>
    T getValue(const std::string& path, T fallback = T{}) const {
        const Json* value = find(atCursor(), parse(path));
        if (!value || value->is_null()) return fallback;
        try {
            if constexpr (std::is_same_v<T, std::string>) {
                return value->is_string() ? value->get<std::string>() : value->dump();
            } else {
                if (value->is_string()) {
                    std::istringstream stream(value->get<std::string>());
                    T converted{};
                    if (stream >> converted) return converted;
                    return fallback;
                }
                return value->get<T>();
            }
        } catch (const std::exception&) {
            return fallback;
        }
    }

    template <typename T>
    bool setValue(const std::string& name, const T& value) {
        Json* current = atCursor();
        if (!current || !current->is_object()) return false;
        Json& child = (*current)[name];
        if (child.is_array()) {
            if (child.empty()) child.push_back(value);
            else child[0] = value;
        } else {
            child = value;
        }
        return true;
    }

    bool remove(const std::string& path) {
        Json* current = atCursor();
        if (!current || !current->is_object()) return false;
        const Segment segment = parse(path);
        auto it = current->find(segment.name);
        if (it == current->end()) return false;
        if (it->is_array()) {
            if (segment.index >= it->size()) return false;
            it->erase(it->begin() + segment.index);
            if (it->empty()) current->erase(it);
        } else if (segment.index == 0) {
            current->erase(it);
        } else {
            return false;
        }
        return true;
    }

private:
    struct Segment {
        std::string name;
        std::size_t index;
    };

    static Segment parse(const std::string& path) {
        const auto open = path.find('[');
        if (open == std::string::npos || path.empty() || path.back() != ']')
            return {path, 0};
        try {
            return {path.substr(0, open), std::stoul(path.substr(open + 1, path.size() - open - 2))};
        } catch (const std::exception&) {
            return {path, 0};
        }
    }

    static const Json* find(const Json* parent, const Segment& segment) {
        if (!parent || !parent->is_object()) return nullptr;
        auto it = parent->find(segment.name);
        if (it == parent->end()) return nullptr;
        if (it->is_array()) return segment.index < it->size() ? &(*it)[segment.index] : nullptr;
        return segment.index == 0 ? &*it : nullptr;
    }

    Json* atCursor() {
        Json* current = &document_;
        for (const auto& segment : cursor_) {
            current = const_cast<Json*>(find(current, segment));
            if (!current) return nullptr;
        }
        return current;
    }

    const Json* atCursor() const {
        const Json* current = &document_;
        for (const auto& segment : cursor_) {
            current = find(current, segment);
            if (!current) return nullptr;
        }
        return current;
    }

    bool append(const std::string& name, const Json& value) {
        Json* current = atCursor();
        if (!current || !current->is_object()) return false;
        auto it = current->find(name);
        if (it == current->end()) {
            (*current)[name] = value;
        } else if (it->is_array()) {
            it->push_back(value);
        } else {
            Json old = *it;
            *it = Json::array({old, value});
        }
        return true;
    }

    Json document_ = Json::object();
    std::vector<Segment> cursor_;
    std::optional<std::filesystem::path> unreadablePath_;
};
