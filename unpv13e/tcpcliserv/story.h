#include <stdio.h>

// 定義選項的結構
typedef struct {
    char description[256];  // 選項描述
    int nextNodeIndex;      // 根據這個選項導向的下一個節點的索引
} Option;

// 定義故事節點的結構
typedef struct {
    char story[1024];   // 故事內容
    Option options[3];  // 每個節點有3個選項
} StoryNode;

// 函數原型聲明
void displayStoryNode(StoryNode node);

/*
StoryNode node1 = {
        "在艾連製造出的巨人骨架保護下，米卡莎與阿爾敏總算從托洛斯特區安然無恙的返回。",
        {
            {"聽從阿爾敏的選擇", 2},
            {"要求前往地下室", 3},
            {"要求回到軍團", 4}
        }
    };
*/

void displayStoryNode(StoryNode node) {
    printf("故事: %s\n", node.story);
    for (int i = 0; i < 3; i++) {
        printf("選項 %d: %s\n", i + 1, node.options[i].description);
    }
}

