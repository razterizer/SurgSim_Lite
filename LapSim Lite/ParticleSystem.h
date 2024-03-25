#pragma once
#include "LiquidVolumes.h"
#include "../../lib/Termin8or/ParticleSystem.h"


struct ParticleFluidHandler : ParticleHandler
{
  ParticleFluidHandler(size_t N_particles)
    : ParticleHandler(N_particles) {}
  
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
  
};
