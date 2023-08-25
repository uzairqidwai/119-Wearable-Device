#ifndef CONFIG_H
#define CONFIG_H

const char* QUESTIONS_CONFIG = R"({
    "question": "Is it sunny outside?",
    "yes": {
        "question": "Do you want to go for a walk?",
        "yes": {
            "question": "Take an umbrella just in case?",
            "yes": {
                "question": "Great! Enjoy your walk."
            },
            "no": {
                "question": "Alright! Stay safe and enjoy."
            }
        },
        "no": {
            "question": "Stay inside and read a book?",
            "yes": {
                "question": "Enjoy your reading!"
            },
            "no": {
                "question": "Maybe watch a movie then?"
            }
        }
    },
    "no": {
        "question": "Do you want to watch a movie?",
        "yes": {
            "question": "A comedy or thriller?",
            "yes": {
                "question": "Enjoy the laughs!"
            },
            "no": {
                "question": "Stay on the edge of your seat!"
            }
        },
        "no": {
            "question": "How about reading a book?",
            "yes": {
                "question": "Great choice!"
            },
            "no": {
                "question": "Perhaps a nap then?"
            }
        }
    }
})";

#endif // CONFIG_H
