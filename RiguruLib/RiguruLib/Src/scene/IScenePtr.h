#pragma once

#include<memory>

class IScene;
class TitleScene;
//シーン用のシェアドポインタ
using IScenePtr = std::shared_ptr<IScene>;