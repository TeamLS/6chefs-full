//
//  EventScriptTask.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/09/05.
//
//

#include "Tasks/EventScriptTask.h"

#include "Scenes/DungeonScene.h"
#include "Scenes/TitleScene.h"

//イベント関数の関数ポインタ格納
const map<string, EventScriptTask::FunctionPointer> EventScriptTask::EVENT_MAP = {
    //命令タイプ
    {"sequence", &EventScriptTask::sequence},    //順番に処理を実行
    {"spawn", &EventScriptTask::spawn},       //同時に処理を実行
    {"repeat", &EventScriptTask::repeat},    //繰り返し実行
    {"flagif", &EventScriptTask::flagif},        //flagによって場合分けして実行
    //イベントタイプ
    {"changeMap", &EventScriptTask::changeMap},//マップ移動
    {"move", &EventScriptTask::move},     //オブジェクトの移動
    {"message", &EventScriptTask::message},  //システムのメッセージ
    {"talk", &EventScriptTask::talk},     //キャラクターの会話
    {"fade", &EventScriptTask::fade},     //画面特殊効果
    {"playSE", &EventScriptTask::playSE},   //効果音再生
    {"playBGM", &EventScriptTask::playBGM}, //BGM再生
    {"control", &EventScriptTask::control}, //操作状態の変更
    {"read", &EventScriptTask::read}      //書物読んでるモード
};

// コンストラクタ
EventScriptTask::EventScriptTask(){
    FUNCLOG
    }

// デストラクタ
EventScriptTask::~EventScriptTask(){FUNCLOG}

// 初期化
bool EventScriptTask::init(DungeonScene* dungeonScene)
{
	FUNCLOG
	if(!dungeonScene) return false;
	
	this->dungeonScene = dungeonScene;
	
	return true;
}

// イベントスクリプトを実行
void EventScriptTask::runEventScript(int eventId)
{
	CCLOG("EVENT ID >> %d", eventId);
	rapidjson::Value& action { EventScriptManager::getInstance()->getScript(eventId) };
	//各命令の処理
    if(!action.IsNull()) this->dealScript(action);
}

//各イベント命令処理の場合分け
void EventScriptTask::dealScript(rapidjson::Value &action)
{
    SizeType len = action.Size();
    cout << "   len=" << len << endl;
    string type;
    for(int i=0;i<len;i++){
        rapidjson::Value& event = action[i];
        type = static_cast<string>(event["type"].GetString());
        cout << "   type=" << type << endl;
        EventScriptTask::FunctionPointer func;
        func = EventScriptTask::EVENT_MAP.at(type);
        if(!func){
            //default :
        } else {
            Ref* target_act = (this->*func)(event);
            if(target_act != nullptr){
                this->layer->runAction(dynamic_cast<FiniteTimeAction*>(target_act));
            }
        }
    }
    return;
}

//event配列actionのVectorを返す配列
cocos2d::Vector<FiniteTimeAction*> EventScriptTask::createActionVec(rapidjson::Value &sub_act)
{
    FUNCLOG
    SizeType len = sub_act.Size();
    cout << "   len=" << len << endl;
    string type;
    Vector<FiniteTimeAction*> acts;
    for(int i=0;i<len;i++){
        type = static_cast<string>(sub_act[i]["type"].GetString());
        cout << "   type=" << type << endl;
        EventScriptTask::FunctionPointer func;
        func = EventScriptTask::EVENT_MAP.at(type);
        if(!func){
            //default :
        } else {
            Ref* target_act = (this->*func)(sub_act[i]);
            if(target_act != nullptr){
                acts.pushBack(dynamic_cast<FiniteTimeAction*>(target_act));
            }
        }
    }
    return acts;
}

// --------------------------------
//       Instruct functions
// --------------------------------

/**
 * run sequence
 * @param type: string >> sequence
 * @param action: array json >> array of instructs
 */
Ref* EventScriptTask::sequence(rapidjson::Value& event)
{
    FUNCLOG
    return static_cast<Ref*>(Sequence::create(this->createActionVec(event["action"])));
}

/**
 * run spawn
 * @param type: string >> spawn
 * @param action: array json >> array of instructs
 */
Ref* EventScriptTask::spawn(rapidjson::Value& event)
{
    FUNCLOG
    return static_cast<Ref*>(Spawn::create(this->createActionVec(event["action"])));
}

/**
 * run repeat
 * @param type: string >> repeat
 * @param loop: int >> loop count
 * @param action: array json >> array of instruts
 */
Ref* EventScriptTask::repeat(rapidjson::Value &event)
{
    FUNCLOG
    Vector<FiniteTimeAction*> repeatAct;
    Vector<FiniteTimeAction*> origin = this->createActionVec(event["action"]);
    int loop = event["loop"].GetInt();
    for(int i=0;i<loop; i++){
        repeatAct.pushBack(origin);
    }
    return static_cast<Ref*>(Sequence::create(repeatAct));
}

/**
 * flagif
 * @param type: string >> flagif
 * @param map: int >> map_id
 * @param flag: int >> flag_id
 * @param true: array json >> array of instructs
 * @param false: array json >> array of instructs
 */
Ref* EventScriptTask::flagif(rapidjson::Value &event)
{
    FUNCLOG
    return nullptr;
}

// --------------------------------
//       Event functions
// --------------------------------
/**
 * change map
 * @param type: string >> changeMap
 * @param ?
 */
Ref* EventScriptTask::changeMap(rapidjson::Value& event)
{
    FUNCLOG
    //とりあえずテストでタイトル画面に移動するように設計してある
    return static_cast<Ref*>(CallFunc::create([=](){Director::getInstance()->replaceScene(TitleScene::createScene());}));
}
/**
 * Move object
 * @param type: string >> move
 * @param object: string >> name of object
 * @param time: string >> time of move
 * @param x: int >> move x points
 * @param y: int >> move y points
 */
Ref* EventScriptTask::move(rapidjson::Value& event)
{
    FUNCLOG
    double scale = 16.0;
    float x = static_cast<float>(event["x"].GetDouble() * scale);
    float y = static_cast<float>(event["y"].GetDouble() * scale);
    return static_cast<Ref*>(TargetedAction::create(this->layer->getChildByName(EventScriptManager::getInstance()->getMapId())->getChildByName(event["object"].GetString()), MoveBy::create(static_cast<float>(event["time"].GetDouble()), Point(x, y))));
}

/**
 * play sounud effect
 * @param type: string >> playSE
 * @param file: string >> filename
 */
Ref* EventScriptTask::playSE(rapidjson::Value& event)
{
    FUNCLOG
    string file = event["file"].GetString();
    cout << "playSE >> " << file << endl;
    return static_cast<Ref*>(CallFunc::create([=](){}));
}

/**
 * play back ground music
 * @param type: string >> playBGM
 * @param file: string >> filename
 */
Ref* EventScriptTask::playBGM(rapidjson::Value &event)
{
    FUNCLOG
    string file = event["file"].GetString();
    cout << "playBGM >> " << file << endl;
    return static_cast<Ref*>(CallFunc::create([=](){}));
    
}

Ref* EventScriptTask::message(rapidjson::Value& event)
{
    FUNCLOG
    return nullptr;
}

Ref* EventScriptTask::talk(rapidjson::Value& event)
{
    FUNCLOG
    return nullptr;
}

Ref* EventScriptTask::fade(rapidjson::Value& event)
{
    FUNCLOG
    return nullptr;
}

Ref* EventScriptTask::control(rapidjson::Value &event)
{
    FUNCLOG
    return nullptr;
}

Ref* EventScriptTask::read(rapidjson::Value &event)
{
    FUNCLOG
    return nullptr;
}