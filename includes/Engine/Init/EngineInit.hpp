/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EngineInit.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/02 13:21:30 by cledant           #+#    #+#             */
/*   Updated: 2017/09/21 17:52:46 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENGINEINIT_HPP
# define ENGINEINIT_HPP

# include "OpenGL/RessourceManager.hpp"
# include "WindowManager/Glfw_manager.hpp"

struct InitValue
{
	InitValue();
	~InitValue();

	enum Scene_type
	{
		NONE,
		INSTANCING_1,
		MULTILIGHT_1,
		DIRECTIONAL_SHADOW_1,
		OMNI_DIRECTIONAL_SHADOW_1,
		SPOT_SHADOW_1,
	};

	InitValue::Scene_type type;
	unsigned long         res_w;
	unsigned long         res_h;
	bool                  fullscreen;
	unsigned long         monitor;
	bool                  vsync;
};

/*
 * Engine Initializations
 */

void InitRunTestInstancing(Glfw_manager &manager, InitValue const &arg);
void InitRunTestMultiLight(Glfw_manager &manager, InitValue const &arg);
void InitRunTestDirectionalShadow(Glfw_manager &manager, InitValue const &arg);
void InitRunTestOmniShadow(Glfw_manager &manager, InitValue const &arg);
void InitRunTestSpotShadow(Glfw_manager &manager, InitValue const &arg);

/*
 * Shader loading
 */

void ShaderLoading(RessourceManager &rm);

#endif
