#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include <iostream>
#include <variant>
#include "lib/Miner.h"

using json = nlohmann::json;

class API {
    public:
        std::string get(const std::string &url) {
            cpr::Response r = cpr::Get(cpr::Url{url});
            if (r.error) {
                throw std::runtime_error("Error occurred: " + r.error.message);
            }
            if (r.status_code != 200) {
                throw std::runtime_error("HTTP request failed with status code: " + std::to_string(r.status_code));
            }
            try {
                return r.text;
            } catch (const std::exception &e) {
                throw std::runtime_error("Failed to parse response: " + std::string(e.what()));
            }
        }

        std::string post(const std::string &url, json &body) {
            cpr::Response r = cpr::Post(
                cpr::Url{url},
                cpr::Header{{"Content-Type", "application/json"}},
                cpr::Body{body.dump()}
            );
            if (r.error) {
                return std::string("Error occurred: ") + r.text;
            }
            if (r.status_code != 200 && r.status_code != 201) {
                return std::string("HTTP request failed ") + std::to_string(r.status_code) + ": " + r.text;
            }
            try {
                return r.text;
            } catch (const std::exception &e) {
                throw std::runtime_error("Failed to parse response: " + std::string(e.what()));
            }
        }
};