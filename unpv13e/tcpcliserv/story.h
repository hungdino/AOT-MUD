
#define MAX_OPTIONS_PER_CHARACTER 3
#define MAX_CHARACTERS_PER_NODE 3
#define OPTION_DESC_LENGTH 256
#define STORY_LENGTH 1024
#define ID_LENGTH 32

// 選項結構
typedef struct {
    char description[OPTION_DESC_LENGTH];
    int nextNodeIndex;
} Option;

// 定義角色結構
typedef struct {
    char ID[ID_LENGTH];
    Option options[MAX_OPTIONS_PER_CHARACTER];
    int numberOfOptions;
} Character;

// 故事節點結構
typedef struct {
    int nodeSeriesNum; // 節點編號
    char story[STORY_LENGTH]; // 故事提示
    Character characters[MAX_CHARACTERS_PER_NODE]; // 儲存角色和各自的選項
    int numberOfCharacters; // 
} StoryNode;

/*
// 全局變數
StoryNode node1 = {
    "在艾連製造出的巨人骨架保護下，米卡莎與阿爾敏總算從托洛斯特區安然無恙的返回，艾連自己從巨人身體中走了出來，並表示他想起了父親曾告訴他所有事情只要回到家裡的地下室就會有所解答。",
    {
        {
            { // 角色1的選項
                {"聽從阿爾敏的選擇", 2},
                {"要求前往地下室", 3}
            },
            2  // 角色1有2个选项
        },
        // 如果需要，可继续添加其他角色及其选项
    },
    1  // 这个节点有1个角色
};
*/