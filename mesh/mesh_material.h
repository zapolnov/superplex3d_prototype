#ifndef __MESH__MESH_MATERIAL_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
#define __MESH__MESH_MATERIAL_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__

/* Includes */

#include <core/common.h>
#include <core/ref_counted.h>
#include <core/pointer.h>
#include <core/trolltech.h>
#include <math/vector4.h>


/* Classes */

/** Rendering material. */
class MeshMaterial : public QObject, public RefCounted
{
	Q_OBJECT
	Q_PROPERTY(QString name READ name() WRITE setName())
	Q_PROPERTY(Vector4 ambientColor READ ambientColor() WRITE setAmbientColor())
	Q_PROPERTY(Vector4 diffuseColor READ diffuseColor() WRITE setDiffuseColor())
	Q_PROPERTY(Vector4 specularColor READ specularColor() WRITE setSpecularColor())
	Q_PROPERTY(float shininess READ shininess() WRITE setShininess())
	Q_PROPERTY(QString diffuseTexture READ diffuseTexture() WRITE setDiffuseTexture())
	Q_PROPERTY(QString specularTexture READ specularTexture() WRITE setSpecularTexture())
	Q_PROPERTY(QString normalMap READ normalMap() WRITE setNormalMap())

public:
	/** Constructor. */
	MESHAPI MeshMaterial();

	/** Destructor. */
	MESHAPI ~MeshMaterial();

	/** Returns name of the material. */
	inline const QString & name() const
	{
		return m_Name;
	}

	/**
	 * Sets name of the material.
	 * @param name Name of the material.
	 */
	inline void setName(const QString & name)
	{
		m_Name = name;
	}

	/** Returns ambient color of the material. */
	inline const Vector4 & ambientColor() const
	{
		return m_AmbientColor;
	}

	/**
	 * Sets ambient color of the material.
	 * @param color Ambient color.
	 */
	inline void setAmbientColor(const Vector4 & color)
	{
		m_AmbientColor = color;
	}

	/** Returns diffuse color of the material. */
	inline const Vector4 & diffuseColor() const
	{
		return m_DiffuseColor;
	}

	/**
	 * Sets diffuse color of the material.
	 * @param color Diffuse color.
	 */
	inline void setDiffuseColor(const Vector4 & color)
	{
		m_DiffuseColor = color;
	}

	/** Returns specular color of the material. */
	inline const Vector4 & specularColor() const
	{
		return m_SpecularColor;
	}

	/**
	 * Sets specular color of the material.
	 * @param color Specular color.
	 */
	inline void setSpecularColor(const Vector4 & color)
	{
		m_SpecularColor = color;
	}

	/** Returns shininess of the material. */
	inline float shininess() const
	{
		return m_Shininess;
	}

	/**
	 * Sets shininess of the material.
	 * @param shininess Shininess.
	 */
	inline void setShininess(float shininess)
	{
		m_Shininess = shininess;
	}

	/** Returns name of the diffuse texture. */
	inline const QString & diffuseTexture() const
	{
		return m_DiffuseTexture;
	}

	/**
	 * Sets name of the diffuse texture.
	 * @param texture Name of the diffuse texture.
	 */
	inline void setDiffuseTexture(const QString & texture)
	{
		m_DiffuseTexture = texture;
	}

	/** Returns name of the specular texture. */
	inline const QString & specularTexture() const
	{
		return m_SpecularTexture;
	}

	/**
	 * Sets name of the specular texture.
	 * @param texture Name of the specular texture.
	 */
	inline void setSpecularTexture(const QString & texture)
	{
		m_SpecularTexture = texture;
	}

	/** Returns name of the normal map. */
	inline const QString & normalMap() const
	{
		return m_NormalMap;
	}

	/**
	 * Sets name of the normal map.
	 * @param texture Name of the normal map.
	 */
	inline void setNormalMap(const QString & texture)
	{
		m_NormalMap = texture;
	}

private:
	QString m_Name;				/**< Name of the material. */
	Vector4 m_AmbientColor;		/**< Ambient color. */
	Vector4 m_DiffuseColor;		/**< Diffuse color. */
	Vector4 m_SpecularColor;	/**< Specular color. */
	float m_Shininess;			/**< Shininess of the material. */
	QString m_DiffuseTexture;	/**< Name of the diffuse texture. */
	QString m_SpecularTexture;	/**< Name of the specular texture. */
	QString m_NormalMap;		/**< Name of the normal map. */
};

/** Pointer to the material. */
typedef Pointer<MeshMaterial> MeshMaterialPtr;

#endif // __MESH__MESH_MATERIAL_H__F8F2FCCB_BAB0_4BCF_9319_2A3869A05769__
