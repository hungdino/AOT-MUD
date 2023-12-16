
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

StoryNode Main_node1 = {
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

StoryNode Main_node2 = {
    2, // nodeSeriesNum
    "阿爾敏表示自己一定可以說服駐紮軍團，在他發表的言論下，果真令駐紮軍團大部分的士兵動搖了，但膽小如鼠的指揮官奇茲已經失去思考的能力，仍執意要下令攻擊，就在千鈞一髮之際，駐紮兵團的總司令達特 · 皮克希斯來到，並制止了奇茲。在聽完艾連所知的所有關於巨人化的情報後，達特決定採用阿爾敏的建議，要利用巨人化後的艾連將托洛斯特區被摧毀的城門洞口補起來。", // story
    {"Armin", "None", "None"}, // characterArray
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
StoryNode Main_node3 = {
    3, // nodeSeriesNum
    "達特發表了激昂的演說穩住了軍心，準備開始作戰，精銳部隊以伊恩為首，眾人趕抵了巨石旁，伊恩向艾倫表示今天將有許多士兵因他而死，請務必懷踹必死的決心出發。艾倫準備變身巨人，菁英部隊射出代表開始的綠色信號"
    ,{"Mikasa","Eren", "None"}, // characterArray
    {   // 艾連的選項
        "在城牆上回頭看了自己家的方向一眼，劃破手掌變身",
        "在城牆上回頭看了自己家的方向一眼，跳向對面的房屋劃破手掌變身",
        "跳向對面的巨人劃破手掌變身"
    },
    {   // 米卡莎的選項
        "跟著精銳部隊前去保護艾倫",
        "跟著精銳部隊前去保護艾倫，並且一路上碎碎念",
        "跟著普通士兵在旁邊誘敵"
    },
    {   // 阿爾敏的選項
        "None",
        "None",
        "None"
    }
};

StoryNode Main_node4 = {
    4, // nodeSeriesNum
    "將巨人引到角落的引誘部隊雖然盡量避免與巨人開戰，還是損失了約兩成的兵力，艾連雖然成功變身為巨人，卻開始向米卡莎展開攻擊，里柯認為行動失敗，向天空發射了表示任務失敗的紅色煙霧彈"
    ,{"Armin","Mikasa","None"}, // characterArray
    {   // 艾連的選項
       "掙扎試圖控制自己的巨人身體(機率失敗)",//這裡在client端roll有沒有成功，沒有就直接傳2給server
        "對周圍的士兵造成混亂",
        "對周圍的士兵造成混亂"
    },
    {   // 米卡莎的選項
        "不認為艾倫會失敗，衝到艾倫的臉上試圖喚醒他",
        "在城牆旁大喊大叫，努力說動艾倫和精英部隊繼續",
        "了解到本次作戰已經失敗，但無法放棄艾倫，頹然在附近閃躲他的攻擊"
    },
    {   // 阿爾敏的選項
        "不認為艾倫會失敗，丟下物品往艾倫的方向衝過去",
        "使用信號工具與指揮官溝通，傳達重要信息或調整策略",
        "None"
    },
};

StoryNode Main_node5 = {
    5, // nodeSeriesNum
    "艾倫往自己臉上揮的那拳讓他停下攻擊，伊恩命令精銳部隊繼續掩護艾連，除去周圍的巨人，改變作戰計畫為保護艾連不被巨人攻擊，因為他認為能巨人化的艾連是人類反擊的唯一希望，阿爾敏看到煙霧後也趕到了艾連所在的地方，得知行動失敗後打算將艾連救出來。\n艾倫此時陷入夢中，眼前是自己的媽媽和米卡莎，忽然聽見了敲門聲，原來是阿爾敏在門外邀請他們兩個一起野餐。但是與此同時窗外又傳來模糊的聲音"
    ,{"Mikasa","Armin", "Eren"}, // characterArray
    {   // 艾連的選項
        "拒絕阿爾敏，今天要吃媽媽煮的東西",
        "同意阿爾敏，和米卡莎一起出門",
        "窗外為什麼又是一個阿爾敏 ? 他似乎很焦急的樣子 ? 和這個阿爾敏去看看好了"
    },
    {   // 米卡莎的選項
        "聽從阿爾敏的建議，去幫助其他人",
        "拒絕阿爾敏，表示自己必須待在艾倫旁邊",
        "拒絕阿爾敏，並阻止他刺入巨人"
    },
    {   // 阿爾敏的選項
        "猜測艾倫可能在後頸的位置，一刀刺入用力巨人後頸左側",
        "猜測艾倫可能在後頸的位置，一刀刺入巨人，但是害怕傷到艾倫所以刺而不重",
        "猶豫了一下"
    }
};

StoryNode Main_node6 = {
    6, // nodeSeriesNum
    "另一邊，被阿爾敏成功喚醒的巨人化艾連將巨石成功舉了起來，並開始向洞口走去，伊恩見狀立馬命令所有精銳部隊拚死掩護艾連，但巨人們卻開始不再關注到處移動的人，直直往艾倫過去，幸好大量精銳部隊也趕到。"
    ,{"Armin","Mikasa","None",}, // characterArray
    {   // 艾連的選項
        "None",
        "None",
        "None"
    },
    {   // 米卡莎的選項
        "以保護艾倫為第一優先，一直在艾倫附近",
        "聽從分配到各方誘敵，但不斷回頭觀察艾倫",
        "聽從分配到各方誘敵，相信其他人會保護好"
    },
    {   // 阿爾敏的選項
        "維持原作戰計畫，所有人掩護艾倫前進",
        "既然巨人不會被吸引那就跳到地上主動接近!一邊大吼一邊戳巨人屁股",
        "既然巨人好像是會被艾倫吸引那就先所有人大喊我是艾倫再跳到地上!"
    }
};

StoryNode Mikasa_not_help_node6 = {
    16, // nodeSeriesNum
    "另一邊，被阿爾敏成功喚醒的巨人化艾連將巨石成功舉了起來，並開始向洞口走去，伊恩見狀立馬命令所有精銳部隊拚死掩護艾連，但巨人們卻開始不再關注到處移動的人，直直往艾倫過去，但可惜剛剛被削減大量人士，只有些許部隊也趕到援助。"
    ,{"Armin","Mikasa","None",}, // characterArray
    {   // 艾連的選項
        "None",
        "None",
        "None"
    },
    {   // 米卡莎的選項
        "以保護艾倫為第一優先，一直在艾倫附近",
        "聽從分配到各方誘敵，但不斷回頭觀察艾倫",
        "聽從分配到各方誘敵，相信其他人會保護好"
    },
    {   // 阿爾敏的選項
        "維持原作戰計畫，所有人掩護艾倫前進",
        "既然巨人不會被吸引那就跳到地上主動接近!一邊大吼一邊戳巨人屁股",
        "既然巨人好像是會被艾倫吸引那就先所有人大喊我是艾倫再跳到地上!"
    }
};

StoryNode Main_node7 = {
    7, // nodeSeriesNum
    "艾連不斷往洞口方向行走，但是此時突然聽見了遠方的尖叫聲，原來是約翰的立體機動裝置壞掉了，然而此時卻有有巨人朝他走來...。"
     ,{"Mikasa","Eren","None"}, // characterArray
    {   // 艾連的選項
        "回頭查看",
        "相信隊友，我已經無路可退",
        "None"
    },
    {   // 米卡莎的選項
        "衝下去把讓帶上來",
        "掩護讓讓他能夠拿到別人的立體機動裝置",
        "None"
    },
    {   // 阿爾敏的選項
        "None",
        "None",
        "None"
    }
};

StoryNode TE_node7 = {
    27, // nodeSeriesNum
    "艾連不斷往洞口方向行走，但是此時突然聽見了遠方的尖叫聲，原來是約翰的立體機動裝置壞掉了，然而此時卻有有金髮女巨人朝他走來...。"
     ,{"Mikasa","Eren","None"}, // characterArray
    {   // 艾連的選項
        "回頭查看",
        "相信隊友，我已經無路可退",
        "None"
    },
    {   // 米卡莎的選項
        "衝下去把讓帶上來",
        "掩護讓讓他能夠拿到別人的立體機動裝置",
        "None"
    },
    {   // 阿爾敏的選項
        "None",
        "None",
        "None"
    }
};

StoryNode Mikasa_not_help_node7 = {
    17, // nodeSeriesNum
    "巨人被吸引的狀況不夠樂觀，人數一直在瘋狂的減少，眼看情勢岌岌可危，隊長伊恩親自以破軍之勢衝向巨人，但也不過是無謂的犧牲，艾倫還在半路上，必須掩護才行"
    ,{"Armin","Mikasa","Eren",}, // characterArray
    {   // 艾連的選項
        "尋找一個相對安全的地點暫時隱蔽，等待局勢有所好轉或援助到達",
        "繼續往前",
        "放棄原計劃，轉而投入保護同伴的戰鬥"
    },
    {   // 米卡莎的選項
        "全力保護艾倫，確保他能夠安全到達目的地",
        "利用環境進行隱蔽式攻擊",
        "迅速組織一支後援小隊，支援正面戰場"
    },
    {   // 阿爾敏的選項
        "發送信號彈請求援助",
        "用周圍的地形和環境，設計一些陷阱或障礙，以減緩巨人的進攻速度",
        "快速制定一個有組織的撤退計劃"
    }
};


StoryNode basement_node2 = {
    22, // nodeSeriesNum
    "害怕於艾倫被軍團擊殺，三人決定還是前往地下室探索秘密。途中，他們必須穿越被巨人佔領的區域，面對潛在的危險和未知的威脅。他們也需要與時間賽跑，以免被其他不明勢力捕獲", // story
    {"Armin","Mikasa","Eren"},  // characterArray
    {   // 艾連的選項
        "帶頭穿越危險區域",
        "保持隱蔽，避免與巨人直接交鋒",
        "設置陷阱分散巨人的注意力"
    },
    {   // 米卡莎的選項
        "緊隨艾倫，隨時準備保護他",
        "負責偵查和提前警報",
        "待命，協助隊伍處理突發狀況"
    },
    {   // 阿爾敏的選項
        "分析環境，尋找安全的穿越方法",
        "協調團隊的行動和溝通",
        "策劃路線和戰術"
    }
};

StoryNode basement_node3 = {
    22, // nodeSeriesNum
    "在三人千辛萬苦到達地下室後，卻發現門和艾倫手上的鑰匙完全對不上。", // story
    {"Armin","Mikasa","Eren"}, // characterArray
    {   // 艾連的選項
        "回到軍團",
        "把鑰匙反過來再試一次",
        "把這個門踹開好了"
    },
    {   // 米卡莎的選項
        "保護艾倫和阿爾敏",
        "尋找通往地下室的其他入口",
        "把這個門踹開好了"
    },
    {   // 阿爾敏的選項
        "安排與軍團的聯繫",
        "拿出鐵絲敲開門",
        "勸說艾倫放棄地下室"
    }
};

StoryNode Mikasa_normal_node3 = {
    22, // nodeSeriesNum
    "米卡莎加入普通士兵，專注於誘導巨人遠離艾倫和巨石的區域。她的行動成功吸引了一部分巨人的注意力，降低了對艾倫的直接威脅。另外一方，艾連雖然成功變身為巨人，卻開始向士兵所在的鐘樓展開攻擊，里柯認為行動失敗，向天空發射了表示任務失敗的紅色煙霧彈", // story
    {"Armin","Mikasa","Eren"}, // characterArray
    {   // 艾連的選項
        "掙扎試圖控制自己的巨人身體",//(成功直接到主節點6)
        "對周圍的士兵造成混亂",
        "對周圍的士兵造成混亂"
    },
    {   // 米卡莎的選項
        "迅速返回艾倫身邊",//(回到主節點5)
        "繼續留在原地，吸引更多巨人的注意力",//(正確)
        "None"
    },
    {   // 阿爾敏的選項
        "試圖直接接近並干預艾倫",
        "協調其他士兵建立防禦陣線",
        "勸說團長繼續計畫"//(正確)
    }
};

StoryNode Mikasa_normal_node4 = {
    22, // nodeSeriesNum
    "阿爾敏成功說服了團長，能巨人化的艾連是人類反擊的唯一希望，艾倫旁邊的士兵一面努力和他周旋，一面盡力抵抗巨人，就在此刻，一個穿著斗篷的身影突然到來", // story
    {"Armin","Mikasa","Eren"}, // characterArray
    {   // 艾連的選項
        "掙扎試圖控制自己的巨人身體",//(成功直接到主節點6)，阿這裡強制會失敗就好
        "對周圍的士兵造成混亂",
        "對周圍的士兵造成混亂"
    },
    {   // 米卡莎的選項
        "與神秘角色合作",
        "直接與神秘角色對抗",//只有這個錯誤
        "與神秘角色溝通，尋求協助"
    },
    {   // 阿爾敏的選項
        "試圖直接接近並干預艾倫",
        "協調其他士兵建立防禦陣線",
        "與神秘角色合作"//(阿爾敏不重要)
    }
};
void traverseNode(StoryNode node) {
    printf("進入節點 %d.\n", node.nodeSeriesNum);
    printf("故事：%s\n", node.story);
    printf("本輪需要做出選擇的玩家為：");
    for (int i = 0; i < MAX_CHARACTERS_PER_NODE; i++)
    {
        if (node.characterArray[i] != "None")
        {
            if (strcmp(node.characterArray[i], "Eren") == 0)
                printf("艾連 ");
            else if (strcmp(node.characterArray[i], "Mikasa") == 0)
                printf("米卡莎 ");
            else if (strcmp(node.characterArray[i], "Armin") == 0)
                printf("阿爾敏 ");
        }
    }
    printf("\n");

    for (int i = 0; i < MAX_CHARACTERS_PER_NODE; ++i) {
        char(*options)[OPTION_DESC_LENGTH]; // Pointer to the options array
        if (strcmp(node.characterArray[i], "Eren") == 0) {
            options = node.Eren;
        }
        else if (strcmp(node.characterArray[i], "Mikasa") == 0) {
            options = node.Mikasa;
        }
        else if (strcmp(node.characterArray[i], "Armin") == 0) {
            options = node.Armin;
        }
        else {
            continue; // If character name does not match, skip to next
        }

        printf("%s's Options:\n", node.characterArray[i]);
        for (int j = 0; j < MAX_OPTIONS_PER_CHARACTER; ++j) {
            printf("  %s\n", options[j]);
        }
    }
}
#pragma once
