#ifndef  __DEFINE_H__
#define  __DEFINE_H__

// Common.hをインクルードするファイル群はこちらをインクルードする
// 最低限のインクルード、変数の初期化を行う

#include <cocos2d.h>
#include <iostream>
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/writer.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/error/en.h"

// ゲームウインドウ横幅、縦幅
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// DEBUGモードの時はdefineしておく
#define DEBUG

using namespace cocos2d;
using namespace std;
using namespace rapidjson;

// メソッドログ
#ifdef DEBUG
#define FUNCLOG CCLOG("%s::%s", typeid(*this).name(), __FUNCTION__);
#else
#define FUNCLOG
#endif

// ゲームシーンタイプ
enum struct SceneType
{
	TITLE,
	DUNGEON,
};

#endif // __DEFINE_H__