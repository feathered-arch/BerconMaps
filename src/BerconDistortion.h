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
#include "BerconSC.h"

extern MCHAR *GetString(int id);

extern HINSTANCE hInstance;

#define DIST_NSUBTEX		3  // Number of subtextures

class BerconDistortion;

class BerconDistortion : public Texmap {
	public:		
		// Distortion		
		BOOL useDistortion{};
		float distortionStr{};

		Point3 getDistVector(ShadeContext& sc);

		// Parameter block
		IParamBlock2	*pblock;	//ref 0
		Texmap			*subtex[DIST_NSUBTEX]{}; //array of sub-materials		
		Interval		ivalid;

		IParamBlock2	*pbXYZ;
		BerconXYZ berconXYZ;

		// Interactive Display		
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

		//TODO: Return the number of sub-textures
		int NumSubTexmaps() override { return DIST_NSUBTEX; }
		//TODO: Return the pointer to the 'i-th' sub-texmap
		Texmap* GetSubTexmap(int i) override { return subtex[i]; }
		void SetSubTexmap(int i, Texmap *m) override;
		TSTR GetSubTexmapSlotName(int i) override;
		
		//From Texmap
		RGBA EvalColor(ShadeContext& sc) override;
		float EvalMono(ShadeContext& sc) override;
		Point3 EvalNormalPerturb(ShadeContext& sc) override;

		//XYZGen *GetTheXYZGen() { return NULL; } 
		
		//TODO: Return anim index to reference index
		int SubNumToRefNum(int subNum) override { return subNum; }
		
		void ReadSXPData(TCHAR *name, void *sxpdata) { }		

		//From Animatable
		Class_ID ClassID() override {return BerconDistortion_CLASS_ID;}		
		SClass_ID SuperClassID() override { return TEXMAP_CLASS_ID; }
		void GetClassName(TSTR& s) override {s = GetString(IDS_BERCON_DIST);}

		RefTargetHandle Clone( RemapDir &remap ) override;
		RefResult NotifyRefChanged(NOTIFY_REF_CHANGED_ARGS) override;

		int NumSubs() override { return 2+DIST_NSUBTEX; }
		Animatable* SubAnim(int i) override; 
		TSTR SubAnimName(int i) override;

		// TODO: Maintain the number or references here 
		int NumRefs() override { return 5; }
		RefTargetHandle GetReference(int i) override;
private:
		void SetReference(int i, RefTargetHandle rtarg) override;
public:
		int	NumParamBlocks() override { return 2; }					// return number of ParamBlocks in this instance
		IParamBlock2* GetParamBlock(int i) override { return i ? pbXYZ : pblock; } // return i'th ParamBlock
		IParamBlock2* GetParamBlockByID(BlockID id) override { return (pblock->ID() == id) ? pblock : pbXYZ; } // return id'd ParamBlock

		void DeleteThis() override { delete this; }		
		
		//Constructor/Destructor
		BerconDistortion();
		virtual ~BerconDistortion();		
};

class BerconDistortionClassDesc : public ClassDesc2 {
public:
	BerconDistortionClassDesc() {}
	virtual ~BerconDistortionClassDesc() {}
	int IsPublic() override { return TRUE; }
	void* Create(BOOL /*loading = FALSE*/) override { return new BerconDistortion(); }
	const TCHAR *	ClassName() override { return GetString(IDS_BERCON_DIST); }
	SClass_ID SuperClassID() override { return TEXMAP_CLASS_ID; }
	Class_ID ClassID() override { return BerconDistortion_CLASS_ID; }
	const TCHAR* Category() override { return TEXMAP_CAT_3D; }

	const TCHAR* InternalName() override { return _M("BerconMapping"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE HInstance() override { return hInstance; }					// returns owning module handle	
};
