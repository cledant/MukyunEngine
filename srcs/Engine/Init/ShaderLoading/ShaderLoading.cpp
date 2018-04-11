/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InitTestDirectionalShadow.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/02 12:14:31 by cledant           #+#    #+#             */
/*   Updated: 2017/09/24 13:31:34 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGL/RessourceManager.hpp"

void ShaderLoading(RessourceManager &rm)
{
	rm.add_shader("BasicProp", "./shaders/BasicProp/BasicProp_vs.glsl",
				  "./shaders/BasicProp/BasicProp_fs.glsl");
	rm.add_shader("DiffuseColored", "./shaders/DiffuseColored/DiffuseColored_vs.glsl",
				  "./shaders/DiffuseColored/DiffuseColored_fs.glsl");
	rm.add_shader("MultiPointDirSpotLight", "./shaders/MultiPointDirSpotLight/MultiPointDirSpotLight_vs.glsl",
				  "./shaders/MultiPointDirSpotLight/MultiPointDirSpotLight_fs.glsl");
	rm.add_shader("ComputeDirLightDepthMap", "./shaders/ComputeDirLightDepthMap/ComputeDirLightDepthMap_vs.glsl",
				  "./shaders/ComputeDirLightDepthMap/ComputeDirLightDepthMap_fs.glsl");
	rm.add_shader("ComputeOmniDepthMap", "./shaders/ComputeOmniDepthMap/ComputeOmniDepthMap_vs.glsl",
				  "./shaders/ComputeOmniDepthMap/ComputeOmniDepthMap_gs.glsl",
				  "./shaders/ComputeOmniDepthMap/ComputeOmniDepthMap_fs.glsl");
	rm.add_shader("ComputeDirShadowMaps", "./shaders/ComputeDirShadowMaps/ComputeDirShadowMaps_vs.glsl",
				  "./shaders/ComputeDirShadowMaps/ComputeDirShadowMaps_fs.glsl");
	rm.add_shader("ComputeOmniShadowMaps", "./shaders/ComputeOmniShadowMaps/ComputeOmniShadowMaps_vs.glsl",
				  "./shaders/ComputeOmniShadowMaps/ComputeOmniShadowMaps_fs.glsl");
	rm.add_shader("ComputeSpotLightShadowMaps",
				  "./shaders/ComputeSpotLightShadowMaps/ComputeSpotLightShadowMaps_vs.glsl",
				  "./shaders/ComputeSpotLightShadowMaps/ComputeSpotLightShadowMaps_fs.glsl");
	rm.add_shader("DisplayDepthMap", "./shaders/DisplayDepthMap/DisplayDepthMap_vs.glsl",
				  "./shaders/DisplayDepthMap/DisplayDepthMap_fs.glsl");
	rm.add_shader("DisplayImage", "./shaders/DisplayImage/DisplayImage_vs.glsl",
				  "./shaders/DisplayImage/DisplayImage_fs.glsl");
	rm.add_shader("MultiPointDirSpotLightWithShadowMap",
				  "./shaders/MultiPointDirSpotLightFusedWithShadowMap/MultiPointDirSpotLightFusedWithShadowMap_vs.glsl",
				  "./shaders/MultiPointDirSpotLightFusedWithShadowMap/MultiPointDirSpotLightFusedWithShadowMap_fs.glsl");
	rm.add_shader("MultiPointDirSpotLightWithShadowMapMultiPass",
				  "./shaders/MultiPointDirSpotLightFusedWithShadowMapMultiPass/MultiPointDirSpotLightFusedWithShadowMapMultiPass_vs.glsl",
				  "./shaders/MultiPointDirSpotLightFusedWithShadowMapMultiPass/MultiPointDirSpotLightFusedWithShadowMapMultiPass_fs.glsl");
}