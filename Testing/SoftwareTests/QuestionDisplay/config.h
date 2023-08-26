#ifndef CONFIG_H
#define CONFIG_H

const char* QUESTIONS_CONFIG = R"({
    "question": "Is it about to rain?",
    "yes_action": "",
    "no_action": "",
    "yes": {
        "question": "Do you want to take an umbrella?",
        "yes_action": "",
        "no_action": "",
        "yes": {
            "question": "Do you have an umbrella?",
            "yes_action": "",
            "no_action": "yellowScreen",
            "yes": {
                "question": "Good Luck!",
                "yes_action": "",
                "no_action": ""
            },
            "no": {
                "question": "Please go buy one!",
                "yes_action": "",
                "no_action": ""
            }
        },
        "no": {
            "question": "Do you have a raincoat?",
            "yes_action": "",
            "no_action": "redScreen",
            "yes": {
                "question": "Good Luck!",
                "yes_action": "",
                "no_action": ""
            },
            "no": {
                "question": "Please go buy one!",
                "yes_action": "",
                "no_action": ""
            }
        }
    }
})";

#endif // CONFIG_H
