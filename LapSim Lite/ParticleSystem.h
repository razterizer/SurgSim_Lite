#pragma once
#include "LiquidVolumes.h"
#include "../../lib/Terminal Text Lib/Screen.h"
#include "../../lib/Core Lib/Rand.h"

struct Particle
{
  Particle() = default;
  
  RC start_pos;
  float time_stamp = 0.f;
  float life_time = 2.f;
  float vel_x = 0.f;
  float vel_y = 0.f;
  float pos_x = 0.f;
  float pos_y = 0.f;
  float g = -30.f;
  bool dead = false;
  
  void init(float time, const RC& pos0, float vx, float vy, float gravity_acc, float spread, float life_t)
  {
    time_stamp = time;
    start_pos = pos0;
    vel_x = vx + spread * (rnd::rand() - 0.5f);
    vel_y = vy + spread * (rnd::rand() - 0.5f);
    g = gravity_acc;
    pos_x = pos0.c;
    pos_y = pos0.r;
    life_time = life_t;
  }
  
  void update(float dt)
  {
    pos_x += vel_x * dt;
    pos_y += vel_y * dt / pix_ar;
    pos_y += g * dt; // y is pointing down. Same direction as r.
  }
  
  bool alive(float time) const
  {
    return time - time_stamp < life_time;
  }
  
  void draw(SpriteHandler& sh, const std::string& str, Text::Color fg_color, Text::Color bg_color, float time) const
  {
    if (alive(time))
      sh.write_buffer(str, std::round(pos_y), std::round(pos_x), fg_color, bg_color);
  }
};

struct ParticleHandler
{
  ParticleHandler(size_t N_particles)
    : num_particles(N_particles), num_particles_active(N_particles), particle_stream(N_particles) {}
  
  void update(const RC& start_pos, bool trigger,
              float vel_x, float vel_y, float g,
              float spread, float life_time, int particle_cluster_size,
              float dt, float time)
  {
    int particle_cluster_idx = 0;
    for (auto& particle : particle_stream)
    {
      if (particle.alive(time))
        particle.update(dt);
      else if (trigger)
      {
        if (!particle.dead)
          particle.init(time, start_pos, vel_x, vel_y, g, spread, life_time);
        if (particle_cluster_idx++ >= particle_cluster_size)
          trigger = false;
      }
    }
  }
  
  void update(const RC& start_pos, bool trigger,
              float vel_x, float vel_y, float g,
              float spread, float life_time, int particle_cluster_size,
              liquids::LiquidVolumes& liquid_volumes, LiquidType liq_type, float droplet_vol,
              float dt, float time)
  {
    int particle_cluster_idx = 0;
    for (auto& particle : particle_stream)
    {
      if (particle.alive(time))
        particle.update(dt);
      else if (trigger)
      {
        if (!particle.dead)
        {
          liquid_volumes.add(liq_type, droplet_vol);
          particle.init(time, start_pos, vel_x, vel_y, g, spread, life_time);
        }
        if (particle_cluster_idx++ >= particle_cluster_size)
          trigger = false;
      }
    }
  }
  
  void draw(SpriteHandler& sh, const std::string& str, Text::Color fg_color, Text::Color bg_color, float time) const
  {
    for (const auto& particle : particle_stream)
      if (!particle.dead)
        particle.draw(sh, str, fg_color, bg_color, time);
  }
  
  void set_num_active_particles(float amount_ratio_active)
  {
    num_particles_active = static_cast<int>(std::round(static_cast<float>(num_particles) * amount_ratio_active));
    for (size_t p_idx = num_particles_active; p_idx < num_particles; ++p_idx)
      particle_stream[p_idx].dead = true;
  }
  
  std::vector<Particle> particle_stream;
  const size_t num_particles = 0;
  
private:
  size_t num_particles_active = 0;
};
