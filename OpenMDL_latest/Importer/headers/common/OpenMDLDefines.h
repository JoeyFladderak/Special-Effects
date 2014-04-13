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
#ifndef _OPENMDL_COMMON_DEFINES_H_
#define _OPENMDL_COMMON_DEFINES_H_

// compiler configuration

#ifdef _MSC_VER
#	define OPENMDL_VISUALSTUDIO
#	define OPENMDL_COMPILER					"MSVC"
#	define OPENMDL_FUNCTION					__FUNCTION__
#	define OPENMDL_FILE						__FILE__
#	define OPENMDL_LINE						__LINE__
#	define OPENMDL_INLINE					__inline
#	define OPENMDL_LIBEXPORT				__declspec(dllexport)
#	define OPENMDL_LIBIMPORT				__declspec(dllimport)

#	pragma warning( disable : 4275 )
#	pragma warning( disable : 4996 )
#	pragma warning( disable : 4251 )
#endif

#ifdef __GNUC__
#	define OPENMDL_GCC
#	define OPENMDL_COMPILER					"GCC"
#	define OPENMDL_FUNCTION					__func__
#	define OPENMDL_FILE						__FILE__
#	define OPENMDL_LINE						__LINE__
#	define OPENMDL_INLINE					inline
#endif



// libexp define

#ifdef OPENMDL_LIBEXP
#	define OPENMDL_EXPORT_FUNCTION			OPENMDL_LIBEXPORT
#else
#	define OPENMDL_EXPORT_FUNCTION			OPENMDL_LIBIMPORT
#endif



// platform configuration

#ifdef _WIN32
#	define OPENMDL_WINDOWS
#endif

#ifdef _WIN64
#	define OPENMDL_WINDOWS
#endif

#ifdef __APPLE__
#	define OPENMDL_OSX
#endif

#ifdef __linux__
#	define OPENMDL_LINUX
#endif



// common helper defines and OpenMDL-specific defines

#define OPENMDL_ALIGNEDMALLOC(x)		_aligned_malloc(x, 64)
#define OPENMDL_ALIGNEDFREE(x)			_aligned_free(x)
#define OPENMDL_SAFE_DELETE(x)			{ if(x) delete x; x = NULL; }
#define OPENMDL_SAFE_ARR_DELETE(x)		{ if(x) delete [] x; x = NULL; }
#define OPENMDL_SAFE_RELEASE(x)			{ if(x) x->Release(); }
#define OPENMDL_SAFE_RELEASE_DELETE(x)	{ if(x) x->Release(); delete x; x = NULL; }
#define OPENMDL_SAFE_FCLOSE(x)			{ if(x) fclose(x); x = NULL; }
#define OPENMDL_SAFE_FREE(x)			{ if(x) free(x); }
#define OPENMDL_BIT(x)					(1 << (x))
#define OPENMDL_VECTOR_ITER(set, var)	for(unsigned int var = 0; var < set.Size(); ++var)
#define OPENMDL_MAP_ITER(type, set)		for(type::iterator it = set.Begin(); it != set.End(); ++it)
#define OPENMDL_BOOL_TO_BYTE(b)			((b) ? 0xFF : 0x00)
#define OPENMDL_BYTE_TO_BOOL(b)			((b) ? true : false)
#define OPENMDL_VERSION_STRING			"0.95.2 beta"
#define OPENMDL_VERSION_INTEGER			952
#define OPENMDL_VERSION_DOUBLE			0.952
#define OPENMDL_VERSION_FLOAT			0.952f
#define OPENMDL_FILE_HEADER_ID			0x4f70656e4d444c30 // "0LDMnepO"
#define OPENMDL_HEADER_OPTIONCOUNT		3
#define OPENMDL_STRLEN					128


// openmdl chunk tags

#define OPENMDL_FILECHUNK_VERTICES		0x0001
#define OPENMDL_FILECHUNK_NORMALS		0x0002
#define OPENMDL_FILECHUNK_TANGENTS		0x0003
#define OPENMDL_FILECHUNK_BINORMALS		0x0012
#define OPENMDL_FILECHUNK_TEXCOORDS		0x0004
#define OPENMDL_FILECHUNK_CAMERAS		0x0005
#define OPENMDL_FILECHUNK_LIGHTS		0x0006
#define OPENMDL_FILECHUNK_JOINTS		0x0007
#define OPENMDL_FILECHUNK_ANIMATIONS	0x0008
#define OPENMDL_FILECHUNK_MATERIALS		0x0009
#define OPENMDL_FILECHUNK_OBJECT		0x0011
#define OPENMDL_FILECHUNK_OPTIONS		0x0012
#define OPENMDL_FILECHUNK_GROUPCOUNT	0x0013
#define OPENMDL_FILECHUNK_NAMETABLE		0x0014


// openmdl option tags

#define OPENMDL_FILEOPTION_TRIANGULATED	0x000A
#define OPENMDL_FILEOPTION_INVERTU		0x000B
#define OPENMDL_FILEOPTION_INVERTV		0x000C
#define OPENMDL_FILEOPTION_SEPMAT		0x000D
#define OPENMDL_FILEOPTION_SEPJNT		0x000E
#define OPENMDL_FILEOPTION_SEPANI		0x000F

#endif