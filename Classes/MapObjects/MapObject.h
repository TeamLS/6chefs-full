//
//  MapObject.h
//  LastSupper
//
//  Created by Kohei on 2015/06/21.
//
//

#ifndef __MAP_OBJECT_H__
#define __MAP_OBJECT_H__

#include "Common.h"

class Light;
class AmbientLightLayer;
class MapObjectList;

class MapObject : public Node
{
// 定数
public:
    enum EventID
    {
        UNDIFINED = -1,
    };
// インスタンス変数
private:
    int eventId { EventID::UNDIFINED };
	Trigger trigger {Trigger::SIZE};
	bool _isHit { false };
	Direction movingDirection {Direction::SIZE};
    Rect collisionRect {Rect::ZERO};
	Light* light { nullptr };
    MapObjectList* objectList { nullptr };
	
// インスタンスメソッド
public:
	MapObject();
	~MapObject();
	void setGridPosition(const Size& mapSize, const Point& mapGridPoint);
	void setEventId(int eventId);
	void setTrigger(Trigger trigger);
	void setHit(bool _isHit);
	void setMovingDirection(Direction direction);
    void setCollisionRect(const Rect& rect);
    void setMapObjectList(MapObjectList* objectList);
    
	void setLight(Light* light, AmbientLightLayer* ambientLightLayer);
	void removeLight();
    void reaction();
	
    Size  getGridSize() const;
	Point getGridPosition(const Size& mapSize) const;
	int getEventId();
	Trigger getTrigger();
	bool isHit() const;
    bool isHit(const Direction& direction) const;
    bool isHit(const Direction (&directions)[2]) const;
	Direction getMovingDirection();
    Rect getCollisionRect() const;
    Point getAdjacentPosition(const Direction& direction) const;
    Point getAdjacentPosition(const Direction (&directions)[2]) const;

    void drawDebugMask(); // デバッグ用マスク
};

#endif // __MAP_OBJECT_H__
