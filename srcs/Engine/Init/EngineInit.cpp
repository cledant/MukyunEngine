/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Glfw_manager.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/03 11:30:26 by cledant           #+#    #+#             */
/*   Updated: 2017/09/24 13:39:59 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Engine/Init/EngineInit.hpp"

InitValue::InitValue()
{
	this->type       = InitValue::Scene_type::NONE;
	this->res_h      = 720;
	this->res_w      = 1280;
	this->fullscreen = false;
	this->monitor    = 0;
	this->vsync      = false;
	this->instance_size.emplace_back(32);
	this->instance_size.emplace_back(32);
	this->instance_size.emplace_back(32);
	this->model_type        = InitValue::Model_type::NONE;
	this->auto_rotate_model = false;
}