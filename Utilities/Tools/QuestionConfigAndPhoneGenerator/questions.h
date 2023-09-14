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
        "no": {
            "question": "CALL 911!",
            "yes_action": "",
            "no_action": "call911"
        }
    }
})";

#endif // CONFIG_H
