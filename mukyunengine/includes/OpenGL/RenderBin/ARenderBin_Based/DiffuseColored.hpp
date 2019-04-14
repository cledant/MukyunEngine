/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DiffuseColored.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/02 11:24:25 by cledant           #+#    #+#             */
/*   Updated: 2017/11/02 11:24:25 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIFFUSECOLORED_HPP
# define DIFFUSECOLORED_HPP

# include <memory>
# include "OpenGL/RenderBin/ARenderBin.hpp"
# include "OpenGL/LightContainer/LightContainer.hpp"

/*
 * Used for debug to display lights
 */

namespace MukyunEngine
{
	class DiffuseColored : public ARenderBin
	{
		public :

			explicit DiffuseColored(ARenderBin::Params const &params);
			~DiffuseColored() override;
			DiffuseColored(DiffuseColored const &src) = delete;
			DiffuseColored &operator=(DiffuseColored const &rhs) = delete;
			DiffuseColored(DiffuseColored &&src) = delete;
			DiffuseColored &operator=(DiffuseColored &&rhs) = delete;

			/*
			 * Draw
			 */

			void draw() override;
			void update(float tick) override;
			void updateVBO() override;
			void flushData() override;

			/*
			 * Getter
			 */

			std::vector<glm::vec3> const &getVectorLightDiffuse() const;
			std::vector<glm::vec3> moveVectorLightDiffuse();
			size_t getCurrentVectorLightDiffuseNumber() const;
			size_t getMaxVectorLightDiffuseNumber() const;
			GLuint getVBOLightDiffuse() const;
			GLuint moveVBOLightDiffuse();

		protected :

			static constexpr float _light_scale = 0.1f;
			std::vector<glm::vec3> _vector_light_diffuse;
			GLuint                 _vbo_light_diffuse;

			/*
			 * Protected functions
			 */

			inline void _allocate_vbo(size_t max_size);
			inline void _update_vector_light_diffuse();
			inline void _update_vao_light_diffuse();
			inline void _update_light_model_matrix();
	};
}
#endif