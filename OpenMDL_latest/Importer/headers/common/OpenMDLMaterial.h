/*************************************************************************************************
Copyright 2010 OpenMDL.org (Eric H. den Boer). All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are
permitted provided that the following conditions are met:

	1. 	Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.

	2. 	The origin of this software must not be misrepresented; you must 
		not claim that you wrote the original software.  If you use this 
		software in a product, an acknowledgment in the product 
		documentation is required.

	3. 	Altered source and binary versions must be plainly marked as such, and must
		not be misrepresented as being the original software.
	   
	4.	Altered file formats/specifications based on OpenMDL but not conforming the
		OpenMDL standard must be clearly marked as such, and must not be misrepresented
		as being the standard OpenMDL File Specification

	5. 	The name of the author may not be used to endorse or promote 
		products derived from this software without specific prior written 
		permission.
		
	6.	Attribution must be given in one of the following manners
		COMMERCIAL USAGE:
			a. 	one of the attribution logos provided with the OpenMDL SDK must be included 
				in a splash screen/startup sequence of your product with a minimum exposure 
				to the user of 3 seconds;
				AND
			b.	one of the attribution logos provided with the OpenMDL SDK must be clearly 
				printed on the box cover of your product;
				OR IF NOT APPLICABLE (i.e. digital distribution, etc.)
			c.	one of the attribution logos provided with the OpenMDL SDK must be 
				included on your product's website.
				
		NON-COMMERCIAL USAGE (demo reel, portfolio, hobby project, educational coursework assignment, etc.):
			d.	one of the attribution logos provided with the OpenMDL SDK, or the URL http://www.openmdl.org
				must be visible to the viewer/user in your project's documentation. Display of the 
				attribution logos in a demo reel video and/or screenshots is appreciated, but not required.
				
	7.	OpenMDL's File Specification may be changed as long as your customized specification is ONLY used 
		for your product.	

THIS SOFTWARE IS PROVIDED BY OPENMDL.ORG (ERIC H. DEN BOER) "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL OPENMDL.ORG (ERIC H. DEN BOER) OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those of the
authors and should not be interpreted as representing official policies, either expressed
or implied, of OpenMDL.org (Eric H. den Boer).

For a License FAQ, please go to http://www.openmdl.org/?page=license
*************************************************************************************************/
#ifndef _OPENMDL_COMMON_MATERIAL_H_
#define _OPENMDL_COMMON_MATERIAL_H_

#include "OpenMDLCommon.h"

namespace OpenMDL
{
	class OPENMDL_EXPORT_FUNCTION Material
	{
	public:
		Material();
		Material(String& name);
		~Material();

		AttributeList&	GetAttributes() { return m_Attributes; }

		const colorf&	GetAmbientColor() { return m_AmbientColor; }
		const colorf&	GetDiffuseColor() { return m_DiffuseColor; }
		const colorf&	GetEmissiveColor() { return m_EmissiveColor; }
		const colorf&	GetTransparentColor() { return m_TransparentColor; }
		const colorf&	GetReflectiveColor() { return m_ReflectiveColor; }
		const colorf&	GetSpecularColor() { return m_SpecularColor; }
		
		float			GetReflectiveCoeff() { return m_ReflectiveCoeff; }
		float			GetTransparentCoeff() { return m_ReflectiveCoeff; }
		float			GetSpecularCoeff() { return m_SpecularCoeff; }
		float			GetGlowCoeff() { return m_GlowCoeff; }
		float			GetDiffuseCoeff() { return m_DiffuseCoeff; }

		float			GetRefractionIndex() { return m_RefractionIndex; }
		unsigned int	GetRefractionLimit() { return m_RefractionLimit; }
		float			GetLightAbsorbance() { return m_LightAbsorbance; }
		float			GetSurfaceThickness() { return m_SurfaceThickness; }
		float			GetShadowAttenuation() { return m_ShadowAttenuation; }
		bool			GetChromaticAberration() { return m_ChromAberration; }
		unsigned int	GetReflectionLimit() { return m_ReflectionLimit; }
		float			GetReflectionSpecularity() { return m_ReflectionSpec; }
		
		String&	GetAmbientMap() { return m_AmbientMap; }
		String&	GetDisplacementMap() { return m_DisplacementMap; }
		String&	GetDiffuseMap() { return m_DiffuseMap; }
		String&	GetEmissiveMap() { return m_EmissiveMap; }
		String&	GetSpecularMap() { return m_SpecularMap; }
		String&	GetNormalMap() { return m_NormalMap; }
		String&	GetOpacityMap() { return m_OpacityMap; }
		String&	GetTransparencyMap() { return m_TransparencyMap; }
		String&	GetReflectionMap() { return m_ReflectionMap; }
		String&	GetGlowMap() { return m_GlowMap; }
		String&	GetRefractionMap() { return m_RefractionMap; }
		String&	GetLightAbsorbanceMap() { return m_LightAbsorbanceMap; }
		String&	GetSurfaceThicknessMap() { return m_SurfaceThicknessMap; }
		String&	GetShadowAttenuationMap() { return m_ShadowAttenuationMap; }
		String&	GetReflectionSpecularityMap() { return m_ReflectionSpecMap; }
		String&	GetShaderFile() { return m_ShaderFile; }
		String&	GetName() { return m_Name; }

		const bool		HasAmbientMap() { return (m_AmbientMap.GetLength() > 0); }
		const bool		HasDisplacementMap() { return (m_DisplacementMap.GetLength() > 0); }
		const bool		HasDiffuseMap() { return (m_DiffuseMap.GetLength() > 0); }
		const bool		HasEmissiveMap() { return (m_EmissiveMap.GetLength() > 0); }
		const bool		HasSpecularMap() { return (m_SpecularMap.GetLength() > 0); }
		const bool		HasNormalMap() { return (m_NormalMap.GetLength() > 0); }
		const bool		HasOpacityMap() { return (m_OpacityMap.GetLength() > 0); }
		const bool		HasTransparencyMap() { return (m_TransparencyMap.GetLength() > 0); }
		const bool		HasReflectionMap() { return (m_ReflectionMap.GetLength() > 0); }
		const bool		HasGlowMap() { return (m_GlowMap.GetLength() > 0); }
		const bool		HasRefractionMap() { return (m_RefractionMap.GetLength() > 0); }
		const bool		HasLightAbsorbanceMap() { return (m_LightAbsorbanceMap.GetLength() > 0); }
		const bool		HasSurfaceThicknessMap() { return (m_SurfaceThicknessMap.GetLength() > 0); }
		const bool		HasShadowAttenuationMap() { return (m_ShadowAttenuationMap.GetLength() > 0); }
		const bool		HasReflectionSpecularityMap() { return (m_ReflectionSpecMap.GetLength() > 0); }
		const bool		HasShaderFile() { return (m_ShaderFile.GetLength() > 0); }

		void			SetAmbientColor(colorf& color) { m_AmbientColor = color; }
		void			SetDiffuseColor(colorf& color) { m_DiffuseColor = color; }
		void			SetEmissiveColor(colorf& color) { m_EmissiveColor = color; }
		void			SetTransparentColor(colorf& color) { m_TransparentColor = color; }
		void			SetReflectiveColor(colorf& color) { m_ReflectiveColor = color; }
		void			SetSpecularColor(colorf& color) { m_SpecularColor = color; }

		void			SetReflectiveCoeff(float coefficient) { m_ReflectiveCoeff = coefficient; }
		void			SetTransparentCoeff(float coefficient) { m_TransparentCoeff = coefficient; }
		void			SetSpecularCoeff(float coefficient) { m_SpecularCoeff = coefficient; }
		void			SetGlowCoeff(float coefficient) { m_GlowCoeff = coefficient; }
		void			SetDiffuseCoeff(float coefficient) { m_DiffuseCoeff = coefficient; }

		void			SetRefractionIndex(float index) { m_RefractionIndex = index; }
		void			SetRefractionLimit(unsigned int limit) { m_RefractionLimit = limit; }
		void			SetLightAbsorbance(float absorbance) { m_LightAbsorbance = absorbance; }
		void			SetSurfaceThickness(float thickness) { m_SurfaceThickness = thickness; }
		void			SetShadowAttenuation(float attenuation) { m_ShadowAttenuation = attenuation; }
		void			SetChromaticAberration(bool abberation) { m_ChromAberration = abberation; }
		void			SetReflectionLimit(unsigned int limit) { m_ReflectionLimit = limit; }
		void			SetReflectionSpecularity(float spec) { m_ReflectionSpec = spec; }
		
		void			SetAmbientMap(String& map) { m_AmbientMap = map; }
		void			SetDisplacementMap(String& map) { m_DisplacementMap = map; }
		void			SetDiffuseMap(String& map) { m_DiffuseMap = map; }
		void			SetEmissiveMap(String& map) { m_EmissiveMap = map; }
		void			SetSpecularMap(String& map) { m_SpecularMap = map; }
		void			SetNormalMap(String& map) { m_NormalMap = map; }
		void			SetOpacityMap(String& map) { m_OpacityMap = map; }
		void			SetTransparencyMap(String& map) { m_TransparencyMap = map; }
		void			SetReflectionMap(String& map) { m_ReflectionMap = map; }
		void			SetGlowMap(String& map) { m_GlowMap = map; }
		void			SetRefractionMap(String& map) { m_RefractionMap = map; }
		void			SetLightAbsorbanceMap(String& map) { m_LightAbsorbanceMap = map; }
		void			SetSurfaceThicknessMap(String& map) { m_SurfaceThicknessMap = map; }
		void			SetShadowAttenuationMap(String& map) { m_ShadowAttenuationMap = map ;}
		void			SetReflectionSpecularityMap(String& map) { m_ReflectionSpecMap = map ;}
		void			SetShaderFile(String& file) { m_ShaderFile = file; }
		void			SetName(String& name) { m_Name = name; }

		void			AddAttribute(String& name, AttributeType type, void* data, unsigned int datasize);
		void			RemoveAttribute(String& name);
		Attribute*		FindAttribute(String& name);

	protected:
		colorf			m_AmbientColor;
		colorf			m_DiffuseColor;
		colorf			m_EmissiveColor;
		colorf			m_TransparentColor;
		colorf			m_ReflectiveColor;
		colorf			m_SpecularColor;

		float			m_ReflectiveCoeff;
		float			m_TransparentCoeff;
		float			m_SpecularCoeff;
		float			m_GlowCoeff;
		float			m_DiffuseCoeff;

		float			m_RefractionIndex;
		unsigned int	m_RefractionLimit;
		float			m_LightAbsorbance;
		float			m_SurfaceThickness;
		float			m_ShadowAttenuation;
		bool			m_ChromAberration;
		unsigned int	m_ReflectionLimit;
		float			m_ReflectionSpec;

		String			m_AmbientMap;
		String			m_DisplacementMap;
		String			m_DiffuseMap;
		String			m_EmissiveMap;
		String			m_SpecularMap;
		String			m_NormalMap;
		String			m_OpacityMap;
		String			m_TransparencyMap;
		String			m_ReflectionMap;
		String			m_GlowMap;
		String			m_RefractionMap;
		String			m_LightAbsorbanceMap;
		String			m_SurfaceThicknessMap;
		String			m_ShadowAttenuationMap;
		String			m_ReflectionSpecMap;
		String			m_ShaderFile;
		String			m_Name;

		AttributeList	m_Attributes;
	};
}

#endif