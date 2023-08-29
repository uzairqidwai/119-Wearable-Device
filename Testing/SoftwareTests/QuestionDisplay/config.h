#ifndef CONFIG_H
#define CONFIG_H

const char* QUESTIONS_CONFIG = R"({
    "question": "Do you enjoy cooking?",
    "yes_action": "",
    "no_action": "",
    "yes": {
        "question": "Do you prefer baking over cooking on the stove?",
        "yes_action": "",
        "no_action": "",
        "yes": {
            "question": "Have you ever baked a cake?",
            "yes_action": "",
            "no_action": "",
            "yes": {
                "question": "You are a baker!",
                "yes_action": "yellowScreen",
                "no_action": ""
            },
            "no": {
                "question": "You are a chef!",
                "yes_action": "",
                "no_action": "redScreen"
            }
        },
        "no": {
            "question": "Have you ever cooked pasta?",
            "yes_action": "",
            "no_action": "",
            "yes": {
                "question": "You are a chef!",
                "yes_action": "redScreen",
                "no_action": ""
            },
            "no": {
                "question": "You are a baker!",
                "yes_action": "",
                "no_action": "yellowScreen"
            }
        }
    },
    "no": {
        "question": "Do you prefer takeout over home-cooked meals?",
        "yes_action": "",
        "no_action": "",
        "yes": {
            "question": "Do you have a favorite restaurant?",
            "yes_action": "",
            "no_action": "",
            "yes": {
                "question": "Glad you have a favourite resturant!",
                "yes_action": "greenScreen",
                "no_action": ""
            },
            "no": {
                "question": "I hope you find a resturant!",
                "yes_action": "",
                "no_action": "blueScreen"
            }
        },
        "no": {
            "question": "Do you have a favorite home-cooked meal?",
            "yes_action": "",
            "no_action": "",
            "yes": {
                "question": "Glad you have a favourite meal!",
                "yes_action": "greenScreen",
                "no_action": ""
            },
            "no": {
                "question": "I hope you find a meal!",
                "yes_action": "",
                "no_action": "blueScreen"
            }
        }
    }
})";

#endif // CONFIG_H
