#ifndef CONFIG_H
#define CONFIG_H

const char* QUESTIONS_CONFIG = R"({
    "question": "Am I breathing?",
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
                    "question": "I've had a seizure. Please touch here to call Jen's preferred emergency contact for help.",
                    "yes_action": "",
                    "no_action": ""
                },
                "no": {
                    "question": "I'm experiencing temporary paralysis. Please touch here to call Jen's preferred emergency contact.",
                    "yes_action": "",
                    "no_action": ""
                }
            },
            "no": {
                "question": "I've had a seizure. Please touch here to call Jen's preferred emergency contact for help.",
                "yes_action": "",
                "no_action": ""
            }
        },
        "no": {
            "question": "Please wait one minute. If I do not regain consciousness, please call 911. A link to call 911 will automatically appear after one minute. Touch here if I wake up.",
            "yes_action": "sendLoc2EmgCnt",
            "no_action": "sendLoc2EmgCnt"
        }
    },
    "no": {
        "question": "Call 911",
        "yes_action": "sendLoc2EmgCnt",
        "no_action": "sendLoc2EmgCnt"
    }
})";

#endif // CONFIG_H
