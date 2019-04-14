

#ifndef APROP_HPP
# define APROP_HPP

# include "GameEntities/Utility/CollisionBox.hpp"
# include "Exceptions/GeneralException.hpp"
# include "glm/gtc/matrix_transform.hpp"
# include "glm/glm.hpp"

namespace MukyunEngine
{
	class AProp
	{
		public :

			enum class eType
			{
					PROP,
			};

			enum class eDamages
			{
					NONE          = 0,
					HALF          = 1,
					SINGLE        = 2,
					INSTANT_DEATH = 10000,
			};

			struct Params
			{
				Params();
				virtual ~Params() = default;

				glm::vec3 model_center;
				glm::vec3 pos;
				glm::vec3 orientation;
				glm::vec3 scale;
				glm::vec3 offset;
				bool      active;
				glm::vec3 cb_half_size;
				eDamages  dmg;
				bool      passthrough;
				bool      auto_rotate;
				glm::vec3 rotation_per_tick;
			};

			explicit AProp(AProp::Params const &params);
			virtual ~AProp() = default;
			AProp(AProp const &src);
			AProp &operator=(AProp const &rhs);

			/*
			 * Virtual fct
			 */

			virtual bool update(float time) = 0;

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
			void setActive(bool value);
			void setDelete(bool value);
			void setPassthrough(bool value);
			void translateObject(glm::vec3 const &vec);
			void scaleObject(glm::vec3 const &vec);
			void rotateObject(glm::vec3 const &vec);
			void setAutoRotation(bool val);
			void setRotationPerTick(glm::vec3 const &vec);

			/*
			 * Getter
			 */

			bool getToUpdate() const;
			float getYaw() const;
			float getPitch() const;
			float getRoll() const;
			glm::vec3 const &getPos() const;
			glm::vec3 const &getScale() const;
			glm::vec3 const &getOffset() const;
			glm::vec3 const &getModelCenter() const;
			bool getActive() const;
			bool getDelete() const;
			bool getPassthrough() const;
			CollisionBox const &getCollisionBox() const;
			eDamages getDamages() const;
			bool getAutoRotation() const;
			glm::vec3 const &getRotationPerTick() const;

		protected :

			float        _yaw;
			float        _pitch;
			float        _roll;
			glm::vec3    _pos;
			glm::vec3    _scale;
			glm::vec3    _offset;
			glm::vec3    _model_center;
			bool         _to_update;
			bool         _active;
			bool         _delete;
			CollisionBox _cb;
			eDamages     _dmg;
			bool         _passthrough;
			bool         _auto_rotate;
			glm::vec3    _rotation_per_tick;
	};
}
#endif