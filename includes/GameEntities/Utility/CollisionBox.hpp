/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CollisionBox.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/09 16:07:59 by cledant           #+#    #+#             */
/*   Updated: 2017/09/10 14:43:03 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLLISIONBOX_HPP
# define COLLISIONBOX_HPP

#include "Exceptions/GeneralException.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Interfaces/ITransformable.hpp"
#include <iostream>
#include <memory>
#include <algorithm>

class CollisionBox : public ITransformable
{
	public :

		struct Resolution
		{
			glm::vec3 pos;
			glm::vec3 delta;
			glm::vec3 normal;
			float     time;
		};

		struct SweepResolution
		{
			Resolution res;
			glm::vec3  pos;
			float      time;
		};

		CollisionBox(glm::vec3 const &pos, glm::vec3 const &half_size);
		CollisionBox(CollisionBox const &src);
		CollisionBox &operator=(CollisionBox const &rhs);
		virtual ~CollisionBox(void);

		/*
		 * Interface ITransformable
		 */

		virtual void translateObject(glm::vec3 const &vec) override;
		virtual void scaleObject(glm::vec3 const &vec) override;
		virtual void rotateObject(glm::vec3 const &vec) override;

		/*
		 * Setter
		 */

		void setPos(glm::vec3 const &pos);
		void setHalfSize(glm::vec3 const &pos);

		/*
		 * Getter
		 */

		glm::vec3 const &getPos(void) const;
		glm::vec3 const &getHalfSize(void) const;

		/*
		 * Collision Functions
		 */

		bool IsPointInBox(glm::vec3 const &pt, Resolution *res) const;
		bool IsBoxInBox(CollisionBox const &box, Resolution *res) const;
		bool IsSegmentInBox(glm::vec3 const &pt, glm::vec3 const &delta,
							glm::vec3 const &padding, Resolution *res) const;
		bool IsBoxInBoxSweep(CollisionBox const &box, glm::vec3 const &delta,
							 SweepResolution *s_res) const;
		bool IsBoxOnBox(CollisionBox const &box) const;

		class InitException : public GeneralException
		{
			public :

				explicit InitException(void);
				virtual        ~InitException(void) throw();
		};

	private :

		glm::vec3 _pos;
		glm::vec3 _half_size;

		inline void _resolution_pt_x(Resolution *res, glm::vec3 const &pt,
									 glm::vec3 const &d, glm::vec3 const &p) const;
		inline void _resolution_pt_y(Resolution *res, glm::vec3 const &pt,
									 glm::vec3 const &d, glm::vec3 const &p) const;
		inline void _resolution_pt_z(Resolution *res, glm::vec3 const &pt,
									 glm::vec3 const &d, glm::vec3 const &p) const;
		inline void _resolution_box_x(Resolution *res, CollisionBox const &box,
									  glm::vec3 const &d, glm::vec3 const &p) const;
		inline void _resolution_box_y(Resolution *res, CollisionBox const &box,
									  glm::vec3 const &d, glm::vec3 const &p) const;
		inline void _resolution_box_z(Resolution *res, CollisionBox const &box,
									  glm::vec3 const &d, glm::vec3 const &p) const;

		static constexpr inline float _sign(float nb);
		static constexpr inline float _max_vec3(glm::vec3 const &vec);
		static constexpr inline float _min_vec3(glm::vec3 const &vec);
};

#endif