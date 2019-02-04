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
			Params();
			virtual ~Params() = default;

			glm::vec3             model_center;
			glm::vec3             pos;
			glm::vec3             orientation;
			glm::vec3             scale;
			glm::vec3             offset;
			bool                  active;
			glm::vec3             cb_half_size;
			ICollidable::eDamages dmg;
			bool                  passthrough;
		};

		explicit Prop(Prop::Params const &params);
		~Prop() override = default;
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
		void setModelCenter(glm::vec3 const &center);

		/*
		 * Getter
		 */


		bool getToUpdate() const;

		/*
		 * Interface IEntity
		 */

		bool update(float time) override;
		void setActive(bool value) override;
		bool getActive() const override;
		void setDelete(bool value) override;
		bool getDelete() const override;

		/*
		 * Interface ITransformable
		 */

		void translateObject(glm::vec3 const &vec) override;
		void scaleObject(glm::vec3 const &vec) override;
		void rotateObject(glm::vec3 const &vec) override;
		float getYaw() const override;
		float getPitch() const override;
		float getRoll() const override;
		glm::vec3 const &getPos() const override;
		glm::vec3 const &getScale() const override;
		glm::vec3 const &getOffset() const override;
		glm::vec3 const &getModelCenter() const override;

		/*
		 * Interface ICollidable
		 */

		CollisionBox const &getCollisionBox() const override;
		ICollidable::eDamages getDamages() const override;
		void setPassthrough(bool value) override;
		bool getPassthrough() const override;

	protected :

		float     _yaw;
		float     _pitch;
		float     _roll;
		glm::vec3 _pos;
		glm::vec3 _scale;
		glm::vec3 _offset;
		glm::vec3 _model_center;
		bool      _to_update;

		//Related to IEntity
		bool _active;
		bool _delete;

		//Related to ICollidable
		CollisionBox          _cb;
		ICollidable::eDamages _dmg;
		bool                  _passthrough;
};

#endif