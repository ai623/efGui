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
		}
	}
}