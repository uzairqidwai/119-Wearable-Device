#ifndef CONFIG_H
#define CONFIG_H

const char* QUESTIONS_CONFIG = R"({
"question": ".",
    "yes_action": "",
    "no_action": "",
    "yes": {
        "question": "Am I breathing",
        "yes_action": "",
        "no_action": "",
        "yes": {
            "question": "Am I conscious?",
            "yes_action": "",
            "no_action": "",
            "yes": {
                "question": "Am I coherent?",
                "yes_action": "",
                "no_action": "",
                "yes": {
                    "question": "Am I moving?",
                    "yes_action": "",
                    "no_action": "",
                    "yes": {
                        "question": "I've had a seizure. My emergency contact has been notified.",
                        "yes_action": "sendLoc2EmgCnt",
                        "no_action": ""
                    },
                    "no": {
                        "question": "I'm experiencing temporary paralysis. My emergency contact has been notified.",
                        "yes_action": "",
                        "no_action": "sendLoc2EmgCnt"
                    }
                },
                "no": {
                    "question": "I've had a seizure. My emergency contact has been notified.",
                    "yes_action": "",
                    "no_action": "sendLoc2EmgCnt"
                }
            },
            "no": {
                "question": "Please wait one minute. If I do not regain consciousness, please call 911.",
                "yes_action": "",
                "no_action": ""
            }
        },
        "no": {
            "question": "Call 911",
            "yes_action": "",
            "no_action": ""
        }
    }
})";

#endif // CONFIG_H
