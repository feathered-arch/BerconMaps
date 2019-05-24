/*
Licensed to the Apache Software Foundation (ASF) under one
or more contributor license agreements. The ASF licenses this
file to you under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance
with the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied.  See the License for the
specific language governing permissions and limitations
under the License.   
*/

#pragma once

#include "BerconCommon.h"
#include "tile.h"

extern MCHAR *GetString(int id);

extern HINSTANCE hInstance;

#define TILE_NSUBTEX		7  // Number of subtextures

class BerconTile;

class BerconTile final : public Texmap, public ResourceMakerCallback {
	public:
		// Tiling parameters
		float tileSize{};	
		
		TileParam tileParam;
		int lockEdge{}, uvChan{}, uvChan2{};

		TilePattern pattern;

		// Tiling		
		bool mappedParameters{};
		TileParam EvalParameters(ShadeContext& sc);

		// Distortion		
		BOOL useDistortion{};
		float distortionStr{};
		Point3 getDistVector(ShadeContext& sc);

		// User Interface
		void EnableStuff(TimeValue t);

		// Parameter block
		IParamBlock2	*pblock;	//ref 1
		IParamBlock2	*pbMap;	//ref PBMAP_REF
		IParamBlock2	*pbXYZ; //ref COORDS

		BerconXYZ berconXYZ;

		Color			 col[3];
		Texmap			*subtex[TILE_NSUBTEX]{}; //array of sub-materials
		BOOL			 mapOn[TILE_NSUBTEX]{};
		static ParamDlg* texoutDlg;
		TextureOutput    *texout;
		Interval		 ivalid;
		//bool viewportPreview;

		inline AColor BerconTile::getColor(ShadeContext &sc, int i)  { return mapOn[i]&&subtex[i] ? subtex[i]->EvalColor(sc): col[i]; }
		inline float  BerconTile::getFloat(ShadeContext &sc, int i)  { return mapOn[i]&&subtex[i] ? subtex[i]->EvalMono(sc): Intens(col[i]); }
		inline Point3 BerconTile::getNormal(ShadeContext &sc, int i) { return mapOn[i]&&subtex[i] ? subtex[i]->EvalNormalPerturb(sc): Point3(0,0,0); }

		// Methods for interactive display
		TexHandle *texHandle;
		Interval texHandleValid;
		void DiscardTexHandle() { if (texHandle) { texHandle->DeleteThis(); texHandle = NULL; } }
		BOOL SupportTexDisplay() override { return TRUE; }
		void ActivateTexDisplay(BOOL onoff) override { if (!onoff) DiscardTexHandle(); }
		DWORD_PTR GetActiveTexHandle(TimeValue t, TexHandleMaker& thmaker) override;	

		//From MtlBase
		ParamDlg* CreateParamDlg(HWND hwMtlEdit, IMtlParams *imp) override;
		BOOL SetDlgThing(ParamDlg* dlg) override;
		void Update(TimeValue t, Interval& valid) override;
		void Reset() override;
		Interval Validity(TimeValue t) override;
		ULONG LocalRequirements(int subMtlNum) override { return berconXYZ.req(); }
		void MappingsRequired(int subMtlNum, BitArray& mapreq, BitArray& bumpreq) override { berconXYZ.map(subMtlNum, mapreq, bumpreq); }

		int NumSubTexmaps() override { return TILE_NSUBTEX; }
		Texmap* GetSubTexmap(int i) override { return subtex[i]; }
		void SetSubTexmap(int i, Texmap *m) override;
		TSTR GetSubTexmapSlotName(int i) override;
		
		//From Texmap
		RGBA EvalColor(ShadeContext& sc) override;
		float EvalMono(ShadeContext& sc) override;
		Point3 EvalNormalPerturb(ShadeContext& sc) override;
		
		int SubNumToRefNum(int subNum) override { return subNum; }
		
		void ReadSXPData(TCHAR *name, void *sxpdata) { }

		//From Animatable
		Class_ID ClassID() override {return BerconTile_CLASS_ID;}		
		SClass_ID SuperClassID() override { return TEXMAP_CLASS_ID; }
		void GetClassName(TSTR& s) override {s = GetString(IDS_BERCON_TILE);}

		RefTargetHandle Clone( RemapDir &remap ) override;
		RefResult NotifyRefChanged(NOTIFY_REF_CHANGED_ARGS) override;

		int NumSubs() override { return 1; }			// Use 1 for parameter block tracked subs
		Animatable* SubAnim(int i) override; 
		TSTR SubAnimName(int i) override;

		int NumRefs() override { return 9; }			// Was 11. Curve is gone, so minus two.
		RefTargetHandle GetReference(int i) override;
private:
		void SetReference(int i, RefTargetHandle rtarg) override;
public:
		int	NumParamBlocks() override { return 3; } // return number of ParamBlocks in this instance
		IParamBlock2* GetParamBlock(int i) override
		{ switch (i) { case 0: return pblock; case 1: return pbMap; case 2: return pbXYZ; } return NULL; }
		IParamBlock2* GetParamBlockByID(BlockID id) override
		{ 
			if (pblock->ID() == id) return pblock;			
			if (pbMap->ID() == id) return pbMap;
			if (pbXYZ->ID() == id) return pbXYZ;
			return NULL;			
		}
		void DeleteThis() override { delete this; }		
		
		//Constructor/Destructor
		BerconTile();
		virtual ~BerconTile();		

		void* GetInterface(ULONG id) override
		{
		/*	if(id == I_RESMAKER_INTERFACE)
				return (void *) (ResourceMakerCallback*) this;
			else*/
				return Texmap::GetInterface(id);
		}
};

class BerconTileClassDesc : public ClassDesc2 {
public:
	BerconTileClassDesc() {}
	virtual ~BerconTileClassDesc() {}
	int IsPublic() override { return TRUE; }
	void* Create(BOOL /*loading = FALSE*/) override { return new BerconTile(); }
	const TCHAR *	ClassName() override { return GetString(IDS_BERCON_TILE); }
	SClass_ID SuperClassID() override { return TEXMAP_CLASS_ID; }
	Class_ID ClassID() override { return BerconTile_CLASS_ID; }
	const TCHAR* Category() override { return TEXMAP_CAT_3D; }
	const TCHAR* InternalName() override { return _M("BerconTile"); } // returns fixed parsable name (scripter-visible name)
	HINSTANCE HInstance() override { return hInstance; }
};
