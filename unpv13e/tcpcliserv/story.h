
#define MAX_OPTIONS_PER_CHARACTER 3
#define MAX_CHARACTERS_PER_NODE 3
#define OPTION_DESC_LENGTH 256
#define STORY_LENGTH 1024
#define ID_LENGTH 32
#define MAX_NODE 14
#define EREN 0
#define MIKASA 1
#define ARMIN 2

int decideNextNode(int currentNode, int numOfNodePlayed) { // currentNode == numOfNodePlayed
    int erenChose[MAX_NODE] = [1, 2, 1, 2, 2, 1, -1, 1, 2, ]; // 固定選項為 1, 2, 3，初始化（還沒選）為 -1，該選項不能選為 0
    int mikasaChose[MAX_NODE] = [1, 2, 1, 1, -1, 1];
    int arminChose[MAX_NODE] = [1, 2, 2, 1, -1, -1, 2];

    int nextNode;

    switch (currentNode) {
    case 1:
        if (mikasaChose[numOfNodePlayed] == 1) {
            if (arminChose[numOfNodePlayed] == 2) {
                nextNode = 2;
            }
            else {
                nextNode = 10;
            }
        }
        else if (mikasaChose[numOfNodePlayed] == 2) {
            if (erenChose[numOfNodePlayed] == 2) {
                nextNode = 10;
            }
            else if (erenChose[numOfNodePlayed] == 1) {
                nextNode = 2;
            }
            else {
                if (arminChose[numOfNodePlayed] == 2) {
                    nextNode = 2;
                }
                else {
                    nextNode = 10;
                }
            }
        }
        break;
    case 2:

    case 3:
        if (erenChose[numOfNodePlayed] == 1) {
            nextNode = 16;//BE特殊
        }
        else if (mikasaChose[numOfNodePlayed] == 2) {
            nextNode = 13;//米咖沙normal
        }
        else {
            nextNode = 4;
        }
        break;
    case 4:
        if (erenChose[numOfNodePlayed] == 1) {
            nextNode = 6;//跳到清醒
        }
        else if (mikasaChose[numOfNodePlayed] == 1) {
            if (arminChose[numOfNodePlayed] == 1) {
                nextNode = 5;//正確到下點
            }
            else {
                nextNode = 14;//神秘人
            }

        }
        else {
            nextNode = 15;//BE一般
        }
        break;
    case 5:
        if (mikasaChose[numOfNodePlayed] != 3 && arminChose[numOfNodePlayed] == 3)
            nextNode = 51;
        else {
            nextNode = 52;
        }
        break;
    case 51:
        if (erenChose[numOfNodePlayed] == 3) {
            if (mikasaChose[5] == 1)
                nextNode = 6;//跳到正確清醒
            if (mikasaChose[5] == 2)
                nextNode = 8;//跳到錯誤清醒
        }
        else {
            nextNode = 17;//BE特殊
        }
        break;
    case 52:
        nextNode = 17;//BE特殊
        break;
    case 6:
        if (mikasaChose[numOfNodePlayed] == 1 && arminChose[numOfNodePlayed] == 1) {
            nextNode = 15;//BE一般
        }
        else if (mikasaChose[numOfNodePlayed] == 1 || arminChose[numOfNodePlayed] == 1) {
            nextNode = 10;
        }
        else if (arminChose[numOfNodePlayed] == 3) {
            nextNode = 9;//TE結局前
        }
        else {
            nextNode = 7;
        }
        break;
    case 7:
        if (erenChose[numOfNodePlayed] == 1) {
            if (mikasaChose[6] == 2 || mikasaChose[8] == 2)
                nextNode = 40;//TE 死亡米咖殺有來
            else if (erenChose[3] == 3)
                nextNode = 41;//TE 死亡
            else
                nextNode = 42;//TE 死亡
        }
        else if (mikasaChose[numOfNodePlayed] == 1) {
            nextNode = 20;//NE1 米卡莎死亡
        }
        else {
            nextNode = 99;//HE
        }

        break;
    case 8:
        if (mikasaChose[numOfNodePlayed] == 1 || arminChose[numOfNodePlayed] == 1) {
            nextNode = 15;//BE一般
        }
        else {
            nextNode = 10;
        }
        break;
    case 9:
        if (erenChose[numOfNodePlayed] == 1) {
            if (mikasaChose[6] == 2 || mikasaChose[8] == 2)
                nextNode = 40;//TE 死亡米咖殺有來
            else if (erenChose[3] == 3)
                nextNode = 41;//TE 死亡
            else
                nextNode = 55;//TE
        }
        else if (mikasaChose[numOfNodePlayed] == 1) {
            nextNode = 20;//NE1 米卡莎死亡
        }
        else {
            nextNode = 99;//HE
        }
        break;
    case 10:
        if (erenChose[numOfNodePlayed] == mikasaChose[numOfNodePlayed] && mikasaChose[numOfNodePlayed] == arminChose[numOfNodePlayed]) {
            if (erenChose[numOfNodePlayed] == 3) {
                nextNode = 20; //NE1 撤退
            }
            else {
                nextNode = 7;
            }
        }
        else {
            nextNode = 15;//BE一般
        }
    case 11:
        if (erenChose[numOfNodePlayed] == mikasaChose[numOfNodePlayed]) {
            if (erenChose[numOfNodePlayed] == 3) {
                nextNode = 15;//BE一般
            }
            else {
                nextNode = 12;
            }
        }
        else {
            nextNode = 15;//BE一般
        }
        break;
    case 12:
        if (erenChose[numOfNodePlayed] == 3 || mikasaChose[numOfNodePlayed] == 3 || arminChose[numOfNodePlayed] != 3) {
            nextNode = 1207;
        }//TE2地下室
        else if (erenChose[numOfNodePlayed] == 1 || arminChose[numOfNodePlayed] == 1) {
            nextNode = 2;//回軍團
        }
        else {
            nextNode = 15;//BE一般
        }
        break;//解釋一下這裡，每個人開門都有機率，要艾倫米卡莎其中一個開成功，阿爾敏不開門才能把門打開，機率失敗默認二
    case 13:
        if (erenChose[numOfNodePlayed] == 1) {
            nextNode = 6;
        }//TE2地下室
        else if (mikasaChose[numOfNodePlayed] == 1) {
            nextNode = 8;//回主點6
        }
        else if (mikasaChose[numOfNodePlayed] == 2 && arminChose[numOfNodePlayed] == 2) {
            nextNode = 14;//冰長
        }
        else {
            nextNode = 15;//BE一般
        }
        break;
    case 14:
        if (mikasaChose[numOfNodePlayed] == 1) {
            nextNode = 15;//BE一般
        }
        else {
            nextNode = 22;//NE 兵長
        }
        break;
    default:
        nextNode = -1; // 回報錯誤：章節不存在

    }
    return nextNode;
}

// 故事節點結構
typedef struct {
    int     nodeSeriesNum;                                          // 節點編號
    char    story[STORY_LENGTH];                                    // 故事提示
    char    characterArray[MAX_CHARACTERS_PER_NODE][ID_LENGTH];     // 這個節點的選擇順序 e.g. {"Eren", "Mikasa", "Armin"}
    char    Eren[MAX_OPTIONS_PER_CHARACTER + 1][OPTION_DESC_LENGTH];    // 儲存艾連的選項
    char    Mikasa[MAX_OPTIONS_PER_CHARACTER + 1][OPTION_DESC_LENGTH];  // 儲存米卡莎的選項
    char    Armin[MAX_OPTIONS_PER_CHARACTER + 1][OPTION_DESC_LENGTH];   // 儲存阿爾敏的選項
} StoryNode;

StoryNode Main_node1 = {
    1, // nodeSeriesNum
    "在艾連製造出的巨人骨架保護下，米卡莎與阿爾敏總算從托洛斯特區安然無恙的返回，艾連自己從巨人身體中走了出來，並表示他想起了父親曾告訴他所有事情只要回到家裡的地下室就會有所解答。", // story
    {"Eren", "Mikasa", "Armin"}, // characterArray
    {   // 艾連的選項
        "None",
        "聽從阿爾敏的選擇",
        "要求前往地下室",
        "要求回到軍團"
    },
    {   // 米卡莎的選項
        "None",
        "聽從阿爾敏的選擇",
        "聽從艾倫的選擇",
        "靜默，但是這樣會平票製作組覺得很煩，那就為了簡單默認選1"//直接給server 1
    },
    {   // 阿爾敏的選項
        "None",
        "決定前往地下室",
        "表示自己一定可以說服駐紮軍團，回到軍團繼續下一步動作",
        "丟個硬幣猜一下"//0.5:1;0.5:2
    } //投票制 
};

StoryNode Main_node2 = {
    2, // nodeSeriesNum
    "阿爾敏表示自己一定可以說服駐紮軍團，在他發表的言論下，果真令駐紮軍團大部分的士兵動搖了，但膽小如鼠的指揮官奇茲已經失去思考的能力，仍執意要下令攻擊，就在千鈞一髮之際，駐紮兵團的總司令達特 · 皮克希斯來到，並制止了奇茲。在聽完艾連所知的所有關於巨人化的情報後，達特決定採用阿爾敏的建議，要利用巨人化後的艾連將托洛斯特區被摧毀的城門洞口補起來。", // story
    {"Armin", "None", "None"}, // characterArray
    {   // 艾連的選項
        "None",
        "None",
        "None",
        "None"
    },
    {   // 米卡莎的選項
        "None",
        "None",
        "None",
        "None"
    },
    {   // 阿爾敏的選項
        "None",
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
        "在城牆上回頭看了自己家的方向一眼，跳向對面的房屋劃破手掌變身",//TE差分，正解
        "跳向對面的巨人劃破手掌變身"//正解
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
    ,{"Armin","Mikasa","Eren"}, // characterArray
    {   // 艾連的選項
       "掙扎試圖控制自己的巨人身體(機率失敗)",//這裡在client端roll有沒有成功，沒有就直接傳2給server
        "對周圍的士兵造成混亂",
        "對周圍的士兵造成混亂"
    },
    {   // 米卡莎的選項
        "不認為艾倫會失敗，衝到艾倫的臉上試圖喚醒他",//正解
        "在城牆旁大喊大叫，努力說動艾倫和精英部隊繼續",
        "了解到本次作戰已經失敗，但無法放棄艾倫，頹然在附近閃躲他的攻擊"
    },
    {   // 阿爾敏的選項
        "不認為艾倫會失敗，丟下物品往艾倫的方向衝過去",//正解
        "使用信號工具與指揮官溝通，傳達重要信息或調整策略",
        "None"
    },
};

StoryNode Main_node5 = {
    5, // nodeSeriesNum
    "艾倫往自己臉上揮的那拳讓他停下攻擊，伊恩命令精銳部隊繼續掩護艾連，除去周圍的巨人，改變作戰計畫為保護艾連不被巨人攻擊，因為他認為能巨人化的艾連是人類反擊的唯一希望，阿爾敏看到煙霧後也趕到了艾連所在的地方，得知行動失敗後打算將艾連救出來。\n艾倫此時陷入夢中，眼前是自己的媽媽和米卡莎，忽然聽見了敲門聲，原來是阿爾敏在門外邀請他們兩個一起野餐。但是與此同時窗外又傳來模糊的聲音"
    ,{"Mikasa","Armin", "None"}, // characterArray
    {   // 艾連的選項
        "None",
        "None",
        "None"
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

StoryNode Main_node5_right = {
    51, // nodeSeriesNum
    "艾倫此時陷入夢中，眼前是自己的媽媽和米卡莎，忽然聽見了敲門聲，原來是阿爾敏在門外邀請他們兩個一起野餐。但是與此同時窗外又傳來模糊的聲音"
    ,{"Eren","None", "None"}, // characterArray
    {   // 艾連的選項
        "拒絕阿爾敏，今天要吃媽媽煮的東西",
        "同意阿爾敏，和米卡莎一起出門",
        "窗外為什麼又是一個阿爾敏 ? 他似乎很焦急的樣子 ? 和這個阿爾敏去看看好了"
    },
    {   // 米卡莎的選項
        "None",
        "None",
        "None"
    },
    {   // 阿爾敏的選項
        "None",
        "None",
        "None"
    }
};

StoryNode Main_node5_false = {
    52, // nodeSeriesNum
    "艾倫此時陷入夢中，眼前是自己的媽媽和米卡莎，忽然聽見了敲門聲，原來是阿爾敏在門外邀請他們兩個一起野餐。但是與此同時窗外又傳來模糊的聲音"
    ,{"Eren","None", "None"}, // characterArray
    {   // 艾連的選項
        "拒絕阿爾敏，今天要吃媽媽煮的東西",
        "同意阿爾敏，和米卡莎一起出門",
        "*****,****,窗外...有聲音嗎?"
    },
    {   // 米卡莎的選項
        "None",
        "None",
        "None"
    },
    {   // 阿爾敏的選項
        "None",
        "None",
        "None"
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
    9, // nodeSeriesNum
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
StoryNode Mikasa_not_help_node6 = {
    8, // nodeSeriesNum
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
StoryNode Mikasa_not_help_node7 = {
    10, // nodeSeriesNum
    "巨人被吸引的狀況不夠樂觀，人數一直在瘋狂的減少，眼看情勢岌岌可危，隊長伊恩親自以破軍之勢衝向巨人，但也不過是無謂的犧牲，艾倫還在半路上，必須掩護才行"
    ,{"Armin","Mikasa","Eren",}, // characterArray
    {   // 艾連的選項
        "尋找一個相對安全的地點暫時隱蔽，等待局勢有所好轉或援助到達",
        "繼續往前",
        "放棄原計劃，轉而投入保護同伴的戰鬥"
    },
    {   // 米卡莎的選項
        "迅速組織一支後援小隊，支援正面戰場",
        "全力保護艾倫，確保他能夠安全到達目的地",
        "掩護附近的同伴撤退"
    },
    {   // 阿爾敏的選項
        "發送信號彈請求援助",
        "用周圍的地形和環境，設計一些陷阱或障礙，以減緩巨人的進攻速度",
        "快速制定一個有組織的撤退計劃"
    }
};


StoryNode basement_node2 = {
    11, // nodeSeriesNum
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
    12, // nodeSeriesNum
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
        "勸說艾倫放棄地下室",
        "拿出鐵絲敲開門"
    }
};

StoryNode Mikasa_normal_node3 = {
    13, // nodeSeriesNum
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
        "協調其他士兵建立防禦陣線",//(正確)
        "勸說團長繼續計畫"
    }
};

StoryNode Mikasa_normal_node4 = {
    14, // nodeSeriesNum
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

StoryNode normal_bad_end = {
    15, // nodeSeriesNum
    "你死了，進擊的巨人世界觀裡，死亡就是如此的不講道理，因為這不是特別關鍵的部分，我甚至懶得幫你額外打特別的文字來紀念遊戲結束，希望下局你至少能出現不一樣的文字，晚安", // story
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
StoryNode castle_bad_end = {
    16, // nodeSeriesNum
    "親愛的玩家，你打算直接在城牆上變成巨人?恭喜成功壓死了附近的士兵和城牆，計畫失敗，至於還在作戰區的艾倫呢?可能是瘋了吧", // story
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

StoryNode sleep_bad_end = {
    17, // nodeSeriesNum
    "艾倫的媽媽和他打了個招呼去午睡了...奇怪，艾倫的媽媽不是死了嗎", // story
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
StoryNode TE_bad_end1 = {
    40, // nodeSeriesNum
    "艾倫回頭查看發生的情況，沒有想到巨人早已經近在眼前，不斷看這裡的米卡莎立刻趕到此處，艾倫鬆了一口氣，但巨人彷彿有神智一般抓住轉瞬即逝的機會出拳...我猜，也許你不該如此依靠米卡莎", // story
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

StoryNode TE_bad_end2 = {
    41, // nodeSeriesNum
    "艾倫回頭查看發生的情況，沒有想到巨人早已經近在眼前，專注於救援讓中的米卡莎沒有注意到艾倫，讓他只能獨自面對。在巨人的重拳來臨之前，他思考著，難道沒有米卡莎我不行嗎?又或許是缺了什麼?", // story
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

StoryNode TE_bad_end3 = {
    42, // nodeSeriesNum
    "艾倫回頭查看發生的情況，沒有想到巨人早已經近在眼前，專注中的米卡莎沒有注意到艾倫，讓他只能獨自面對，艾倫想起了自己跳下城牆時回望家的那一眼，毅然爆發出力量將石頭往門上一丟，同時重重往巨人臉上揍下去。可惜巨人提前扣住了他的手，也許一切就到這裡了。\n你已經很接近真相了，但還缺一點點東西，是運氣，或是天馬行空的想像力?", // story
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

StoryNode HE = {
    99, // nodeSeriesNum
    "艾倫終於將石塊堵在門口，一直注意此處的米卡莎迅速趕到將艾蓮從巨人中拉了出來，調查兵團與駐紮兵團精銳部隊的合力下，眾人清除了托洛斯特區內的所有巨人，並抓獲了兩隻分別為四公尺與七公尺級的巨人，這是人類首次抵禦巨人入侵成功的壯舉，但是共有207人死亡或失蹤，897人輕重傷\n恭喜你抵達了原作中的HE結局，拯救了特洛斯特區!不過我們埋了一些隱藏的彩蛋要試看看嗎", // story
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
StoryNode TE = {
    55, // nodeSeriesNum
    "艾倫回頭查看發生的情況，沒有想到巨人早已經近在眼前，專注於救援讓(差分:因救援讓死亡)中的米卡莎沒有注意到艾倫，讓他只能獨自面對，艾倫想起了自己跳下城牆時回望家的那一眼，毅然爆發出力量將石頭往門上一丟，同時重重往金髮女巨人臉上揍下去。在艾倫觸碰到金髮巨人的一瞬，他忽然覺得自己多出了奇怪的力量。而同一時間趕來的米卡莎和阿爾敏，卻看見破損的城牆上奇異的人臉...\n歡迎來到遊戲的終章，親愛的玩家，祝你玩得開心", // story
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

StoryNode NE = {
    20, // nodeSeriesNum
    "艾倫終於將石塊堵在門口，調查兵團與駐紮兵團精銳部隊的合力下，眾人清除了托洛斯特區內的所有巨人，並抓獲了兩隻分別為四公尺與七公尺級的巨人，這是人類首次抵禦巨人入侵成功的壯舉，但是共有207人死亡或失蹤，897人輕重傷，只可惜，一位拯救摯友的女戰士永遠停在了那片戰場。", // story
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

StoryNode TE2 = {
    1207, // nodeSeriesNum
    "艾倫打開了地下室，看見了他理應看見的世界真相。\n\n\n\n\n\n但我不能劇透，你也想知道世界的真相的話，可以打開第三季或去看完結篇", // story
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

StoryNode NE = {
    22, // nodeSeriesNum
    "原來是緊急趕回的調查兵團，很遺憾，他們似乎認為這個計畫並不適合執行，艾倫被救了回來，但也僅此而已了", // story
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
