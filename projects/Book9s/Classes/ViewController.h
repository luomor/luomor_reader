//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9秒小说阅读器在线讨论 http://www.9miao.com/group-40-59173.html
#ifndef __HelloCpp__ViewController__
#define __HelloCpp__ViewController__

#include <iostream>
//#include "cocos2d.h"
#include "PageHome.h"
#include "PageReader.h"
#include "PageBookStorage.h"

USING_NS_CC;

class ViewController: public CAViewController
{
	BookDB *m_pBookDB;
public:

    static CAWindow* createWindow();
};


#endif /* defined(__HelloCpp__ViewController__) */
