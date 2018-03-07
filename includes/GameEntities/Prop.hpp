/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Prop.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cledant <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/06 09:38:15 by cledant           #+#    #+#             */
/*   Updated: 2017/11/06 09:38:15 by cledant          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROP_HPP
# define PROP_HPP

# include "Interfaces/IEntity.hpp"
# include "Interfaces/ITransformable.hpp"
# include "Interfaces/ICollidable.hpp"
# include "OpenGL/RenderBin/ARenderBin.hpp"
# include "GameEntities/Utility/CollisionBox.hpp"
# include "Exceptions/GeneralException.hpp"
# include "glm/gtc/matrix_transform.hpp"
# include "glm/glm.hpp"
# include <iostream>
# include <vector>

class Prop : public IEntity, public ITransformable, public ICollidable
{
	public :

		struct Params
		{
			Params(void);
			~Params(void);

			ARenderBin           *render_bin;
			glm::vec3            pos;
			glm::vec3            orientation;
			glm::vec3            scale;
			glm::vec3            offset;
			bool                 auto_rotation;
			bool                 active;
			glm::vec3            cb_half_size;
			ICollidable::Damages dmg;
			bool                 passthrough;
		};

		Prop(Prop::Params const &params);
		virtual ~Prop(void);
		Prop(Prop const &src);
		Prop &operator=(Prop const &rhs);

		/*
		 * Setter
		 */

		void setPosition(glm::vec3 const &pos);
		void setScale(glm::vec3 const &scale);
		void setYaw(float yaw);
		void setPitch(float pitch);
		void setRoll(float roll);
		void setOffset(glm::vec3 const &offset);

		/*
		 * Getter
		 */

		ARenderBin const *getRenderBin(void) const;
		float getYaw(void) const;
		float getPitch(void) const;
		float getRoll(void) const;
		glm::vec3 const &getPos(void) const;
		glm::vec3 const &getScale(void) const;
		glm::vec3 const &getOffset(void) const;
		glm::mat4 const &getTotalMatrix(void) const;
		bool getAutoRotation(void) const;

		/*
		 * Interface IEntity
		 */

		virtual void update(float time);
		virtual void requestDraw(void);
		virtual void setActive(bool value);
		virtual bool getActive(void) const;

		/*
		 * Interface ITransformable
		 */

		virtual void translateObject(glm::vec3 const &vec);
		virtual void scaleObject(glm::vec3 const &vec);
		virtual void rotateObject(glm::vec3 const &vec);

		/*
		 * Interface ICollidable
		 */

		virtual CollisionBox const &getCollisionBox(void) const;
		virtual Damages getDamages(void) const;
		virtual void setPassthrough(bool value);
		virtual bool getPassthrough(void) const;

		class InitException : public GeneralException
		{
			public :

				explicit InitException(void);
				virtual ~InitException(void) throw();
		};

	protected :

		ARenderBin *_render_bin;
		float      _yaw;
		float      _pitch;
		float      _roll;
		glm::vec3  _pos;
		glm::vec3  _scale;
		glm::vec3  _offset;
		glm::mat4  _total;
		bool       _auto_rotation;

		//Related to IEntity
		bool _active;

		//Related to ICollidable
		CollisionBox         _cb;
		ICollidable::Damages _dmg;
		bool                 _passthrough;
};

#endif