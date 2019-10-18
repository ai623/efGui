#include "efD3d11Desc.h"

namespace efgui
{
	namespace efd3d11
	{
		namespace efdesc
		{
			const D3D11_BUFFER_DESC gefVertexDesc{
			0,															//UINT ByteWidth;
			D3D11_USAGE_IMMUTABLE,										//D3D11_USAGE Usage;
			D3D11_BIND_VERTEX_BUFFER,									//UINT BindFlags;
			0,															//UINT CPUAccessFlags;

			0,															//UINT MiscFlags;
			0,															//UINT StructureByteStride;
			};

			const D3D11_BUFFER_DESC gefIndexDesc{
			0,															//UINT ByteWidth;
			D3D11_USAGE_IMMUTABLE,										//D3D11_USAGE Usage;
			D3D11_BIND_VERTEX_BUFFER,									//UINT BindFlags;
			0,															//UINT CPUAccessFlags;
			0,															//UINT MiscFlags;
			0,															//UINT StructureByteStride;
			};

			const D3D11_BUFFER_DESC gefConstantDesc{
			0,															//UINT ByteWidth;
			D3D11_USAGE_IMMUTABLE,										//D3D11_USAGE Usage;
			D3D11_BIND_CONSTANT_BUFFER,									//UINT BindFlags;
			0,															//UINT CPUAccessFlags;
			0,															//UINT MiscFlags;
			0,															//UINT StructureByteStride;
			};
	
			//LPCSTR                     SemanticName;
			//UINT                       SemanticIndex;
			//DXGI_FORMAT                Format;
			//UINT                       InputSlot;
			//UINT                       AlignedByteOffset;
			//D3D11_INPUT_CLASSIFICATION InputSlotClass;
			//UINT                       InstanceDataStepRate;
			const D3D11_INPUT_ELEMENT_DESC gefElePOSITIONf2[]		
			{
				{"POSITION",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 }
			};

			
			const D3D11_SAMPLER_DESC gefSamplerDesc
			{
				D3D11_FILTER_MIN_MAG_MIP_LINEAR,		//D3D11_FILTER Filter;
				D3D11_TEXTURE_ADDRESS_BORDER,			//D3D11_TEXTURE_ADDRESS_MODE AddressU;
				D3D11_TEXTURE_ADDRESS_BORDER,			//D3D11_TEXTURE_ADDRESS_MODE AddressV;
				D3D11_TEXTURE_ADDRESS_BORDER,			//D3D11_TEXTURE_ADDRESS_MODE AddressW;
				0,										//FLOAT MipLODBias;
				0,										//UINT MaxAnisotropy;
				D3D11_COMPARISON_NEVER,					//D3D11_COMPARISON_FUNC ComparisonFunc;
				{1,1,1,0},								//FLOAT BorderColor[4];
				0,										//FLOAT MinLOD;
				D3D11_FLOAT32_MAX						//FLOAT MaxLOD;
			};
		}
	}
}