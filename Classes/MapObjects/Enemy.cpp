//
//  Enemy.cpp
//  LastSupper
//
//  Created by Kohei on 2015/11/18.
//
//

#include "MapObjects/Enemy.h"

// コンストラクタ
Enemy::Enemy() {FUNCLOG};

// デストラクタ
Enemy::~Enemy() {FUNCLOG};

// 初期化
bool Enemy::init()
{
    
    return false;
}

// 敵IDを取得
int Enemy::getEnemyId() const
{
    return this->enemyId;
}

// マップに配置された時
void Enemy::onEnterMap(const Point& gridPosistion)
{
    
}

// 主人公一行が移動した時
void Enemy::onPartyMoved(const Point& gridPosition)
{
    
}