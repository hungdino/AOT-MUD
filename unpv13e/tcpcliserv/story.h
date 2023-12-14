
#define MAX_OPTIONS_PER_CHARACTER 3
#define MAX_CHARACTERS_PER_NODE 3
#define OPTION_DESC_LENGTH 256
#define STORY_LENGTH 1024
#define ID_LENGTH 32

// 故事節點結構
typedef struct {
    int     nodeSeriesNum;                                          // 節點編號
    char    story[STORY_LENGTH];                                    // 故事提示
    char    characterArray[MAX_CHARACTERS_PER_NODE][ID_LENGTH];     // 這個節點的選擇順序 e.g. {"Eren", "Mikasa", "Armin"}
    char    Eren[MAX_OPTIONS_PER_CHARACTER][OPTION_DESC_LENGTH];    // 儲存艾連的選項
    char    Mikasa[MAX_OPTIONS_PER_CHARACTER][OPTION_DESC_LENGTH];  // 儲存米卡莎的選項
    char    Armin[MAX_OPTIONS_PER_CHARACTER][OPTION_DESC_LENGTH];   // 儲存阿爾敏的選項
} StoryNode;

StoryNode node1 = {
    1, // nodeSeriesNum
    "在艾連製造出的巨人骨架保護下，米卡莎與阿爾敏總算從托洛斯特區安然無恙的返回，艾連自己從巨人身體中走了出來，並表示他想起了父親曾告訴他所有事情只要回到家裡的地下室就會有所解答。", // story
    {"Eren", "Mikasa", "Armin"}, // characterArray
    {   // 艾連的選項
        "聽從阿爾敏的選擇",
        "要求前往地下室",
        "要求回到軍團"
    },
    {   // 米卡莎的選項
        "聽從阿爾敏的選擇",
        "聽從艾倫的選擇",
        "阿巴阿巴這個不是我的艾倫，砍砍砍"
    },
    {   // 阿爾敏的選項
        "決定前往地下室",
        "真的可以說服駐紮軍團嗎？我們討論一下",
        "表示自己一定可以說服駐紮軍團，回到軍團繼續下一步動作。"
    }
};

StoryNode node2 = {
    2, // nodeSeriesNum
    "阿爾敏表示自己一定可以說服駐紮軍團，在他發表的言論下，果真令駐紮軍團大部分的士兵動搖了，但膽小如鼠的指揮官奇茲已經失去思考的能力，仍執意要下令攻擊，就在千鈞一髮之際，駐紮兵團的總司令達特 · 皮克希斯來到，並制止了奇茲。在聽完艾連所知的所有關於巨人化的情報後，達特決定採用阿爾敏的建議，要利用巨人化後的艾連將托洛斯特區被摧毀的城門洞口補起來。", // story
    {"Eren", "None", "None"}, // characterArray
    {   // 艾連的選項
        "None",
        "None",
        "None"
    },
    {   // 米卡莎的選項
        "None",
        "None",
        "None"
    },
    {   // 阿爾敏的選項
        "讓一群人聚集在石頭的另一邊引誘大部分的巨人到角落，另外再派少數精銳保護艾連",
        "讓一群精銳在石頭的另一邊發出大量聲響和快速移動吸引巨人，主要部隊在艾倫周圍當人牆",
        "表示自己一定可以說服駐紮軍團，回到軍團繼續下一步動作。"
    }
};

void traverseOptions(StoryNode node) {
    for (int i = 0; i < MAX_CHARACTERS_PER_NODE; ++i) {
        char (*options)[OPTION_DESC_LENGTH]; // Pointer to the options array
        if (strcmp(node.characterArray[i], "Eren") == 0) {
            options = node.Eren;
        } else if (strcmp(node.characterArray[i], "Mikasa") == 0) {
            options = node.Mikasa;
        } else if (strcmp(node.characterArray[i], "Armin") == 0) {
            options = node.Armin;
        } else {
            continue; // If character name does not match, skip to next
        }

        printf("%s's Options:\n", node.characterArray[i]);
        for (int j = 0; j < MAX_OPTIONS_PER_CHARACTER; ++j) {
            printf("  %s\n", options[j]);
        }
    }
}
