
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

/*
printf("Node %d: %s\n", node1.nodeSeriesNum, node1.story);
printf("Eren's Options: %s, %s, %s\n", node1.Eren[0], node1.Eren[1], node1.Eren[2]);
*/