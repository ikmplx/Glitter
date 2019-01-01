#pragma once

#define _ENABLE_EXTENDED_ALIGNED_STORAGE

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#pragma warning(push, 0) 
#include "imgui.h"
#pragma warning(pop)

// Bullet
#pragma warning( push )
#pragma warning( disable : 5033)
#pragma warning( disable : 4706 )
#pragma warning( disable : 4305 )
#pragma warning( disable : 26451 )
#include <btBulletDynamicsCommon.h>
#pragma warning( pop )

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <string>
#include <memory>
#include <typeindex>
#include <memory>
#include <bitset>
#include <algorithm>
#include <optional>
#include <array>
