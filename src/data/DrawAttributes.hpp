/******************************************************************************************************
 * Tile Map Editor
 * Copyright (C) 2009-2014 Brandon Whitehead (tricksterguy87[AT]gmail[DOT]com)
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * excluding commercial applications, and to alter it and redistribute it freely,
 * subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented;
 *    you must not claim that you wrote the original software.
 *    An acknowledgement in your documentation and link to the original version is required.
 *
 * 2. Altered source versions must be plainly marked as such,
 *    and must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 ******************************************************************************************************/

#ifndef DRAW_ATTRIBUTES_HPP
#define DRAW_ATTRIBUTES_HPP

#include <cstdint>

class DrawAttributes
{
  public:
  DrawAttributes()
  : depth(0), x(0), y(0), origin_x(0), origin_y(0), scale_x(1), scale_y(1), rotation(0), opacity(100), blend_mode(0),
    blend_color(-1)
  {
  }
  DrawAttributes(int32_t z)
  : depth(z), x(0), y(0), origin_x(0), origin_y(0), scale_x(1), scale_y(1), rotation(0), opacity(100), blend_mode(0),
    blend_color(-1)
  {
  }
  int32_t GetX() const { return x; }
  int32_t GetY() const { return y; }
  int32_t GetOriginX() const { return origin_x; }
  int32_t GetOriginY() const { return origin_y; }
  float GetScaleX() const { return scale_x; }
  float GetScaleY() const { return scale_y; }
  int32_t GetDepth() const { return depth; }
  void SetDepth(int32_t _depth) { depth = _depth; }
  void GetPosition(int32_t& x, int32_t& y) const;
  void SetPosition(int32_t x, int32_t y);
  void GetOrigin(int32_t& x, int32_t& y) const;
  void SetOrigin(int32_t x, int32_t y);
  void GetScale(float& x, float& y) const;
  void SetScale(float x, float y);
  float GetOpacity() const { return opacity; }
  void SetOpacity(float _opacity) { opacity = _opacity; }
  float GetRotation() const { return rotation; }
  void SetRotation(float _rotation) { rotation = _rotation; }
  uint32_t GetBlendMode() const { return blend_mode; }
  void SetBlendMode(uint32_t mode) { blend_mode = mode; }
  uint32_t GetBlendColor() const { return blend_color; }
  void SetBlendColor(uint32_t color) { blend_color = color; }

  protected:
  int32_t depth;
  int32_t x, y;
  int32_t origin_x, origin_y;
  float scale_x, scale_y;
  float rotation;
  float opacity;
  uint32_t blend_mode;
  uint32_t blend_color;
};

int ZDepthCompare(const DrawAttributes& a, const DrawAttributes& b);

#endif
