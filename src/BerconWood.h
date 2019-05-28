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

extern MCHAR *GetString(int id);

extern HINSTANCE hInstance;

#define WOOD_NSUBTEX = 21

class BerconWood final : public Texmap, public ResourceMakerCallback {
	private:
		static const int NSUBTEX = 21;
		static const int NCOLS = 3;
		static const int NUMREF = 25; //changed to 25 from 27 because Curve is gone

	public:
		BOOL /*useCurve,*/ useDistortion, lockGrain;
		float woodSize;
		float grainAmount, grainFreq;

		int woodType;
		float randSeed;
		int samples;

		float lowTresh;
		float highTresh;
		float skew;
		float widthVar;
		float gainVar;
		
		float trunkStr;	
		float trunkFreq;
		
		float radialStr;
		float radialFreq;
		float radialZ;

		float angleStr;
		float angleFreq;
		float angleRad;

		// Distortion		
		float distortionStr;
		void applyDistortion(ShadeContext& sc, Point3& p);

		// User Interface
		void EnableStuff();

		BerconXYZ berconXYZ;

		// Parameter block
		IParamBlock2	*pbXYZ;		//ref 0
		IParamBlock2	*pblock;	//ref 1
	//	IParamBlock2	*pbCurve;	//ref CURVEPB_REF
		IParamBlock2	*pbMap;		//ref PBMAP_REF

		Color			 col[NCOLS];
		Texmap			*subtex[NSUBTEX]; //array of sub-materials
		BOOL			mapOn[NSUBTEX];

		static ParamDlg* texoutDlg;
		TextureOutput   *texout;
		Interval		ivalid;

		bool mappedParameters;
		WoodParam EvalParameters(ShadeContext& sc);

		// Curve
	//	ICurveCtl* curve;				
		// From ResourceMakerCallback		
	/*	BOOL SetCustomImageList(HIMAGELIST &hCTools,ICurveCtl *pCCtl) { return TRUE; };
		BOOL GetToolTip(int iButton, TSTR &ToolTip,ICurveCtl *pCCtl) { return TRUE; };
		void ResetCallback(int curvenum, ICurveCtl *pCCtl) { ICurve *pCurve = NULL; pCurve = pCCtl->GetControlCurve(curvenum); if(pCurve) { pCurve->SetNumPts(2); NewCurveCreatedCallback(curvenum, pCCtl); }}
		void NewCurveCreatedCallback(int curvenum, ICurveCtl *pCCtl) {
			ICurve *pCurve = NULL; pCurve = pCCtl->GetControlCurve(curvenum); TimeValue t = GetCOREInterface()->GetTime();
			CurvePoint pt = pCurve->GetPoint(t,0); pt.p.y = 0.f; pCurve->SetPoint(t,0,&pt);
			pCurve->SetPenProperty( RGB(0,0,0)); pCurve->SetDisabledPenProperty( RGB(128,128,128));		
			pt = pCurve->GetPoint(t,1); pt.p.y = 1.f; pCurve->SetPoint(t,1,&pt);
		}*/

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

		int NumSubTexmaps() override { return NSUBTEX; }
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
		Class_ID ClassID() override {return BerconWood_CLASS_ID;}		
		SClass_ID SuperClassID() override { return TEXMAP_CLASS_ID; }
		void GetClassName(TSTR& s) override {s = GetString(IDS_BERCON_WOOD);}

		RefTargetHandle Clone( RemapDir &remap ) override;
		RefResult NotifyRefChanged(NOTIFY_REF_CHANGED_ARGS) override;

		int NumSubs() override { return 1; }					// We return one when paramblocks are in use
		Animatable* SubAnim(int i) override; 
		TSTR SubAnimName(int i) override;

		int NumRefs() override { return NUMREF-2; }			// Minus two for custom curve being gone
		RefTargetHandle GetReference(int i) override;
private:
		void SetReference(int i, RefTargetHandle rtarg) override;
public:
		int	NumParamBlocks() override { return 3; }
		IParamBlock2* GetParamBlock(int i) override
		{ switch (i) { case 0: return pblock; /*case 1: return pbCurve;*/ case 2: return pbMap; case 3: return pbXYZ; } return NULL; }
		IParamBlock2* GetParamBlockByID(BlockID id) override
		{ 
			if (pblock->ID() == id) return pblock;
		//	if (pbCurve->ID() == id) return pbCurve;
			if (pbMap->ID() == id) return pbMap;
			if (pbXYZ->ID() == id) return pbXYZ;			
			return NULL;			
		}

		void DeleteThis() override { delete this; }		
		
		//Constructor/Destructor
		BerconWood();
		virtual ~BerconWood();		
		
		void* GetInterface(ULONG id) override
		{
			/*	if(id == I_RESMAKER_INTERFACE)
					return (void *) (ResourceMakerCallback*) this;
				else*/
			return Texmap::GetInterface(id);
		}
};

class BerconWoodClassDesc : public ClassDesc2  {
public:
	int IsPublic() override { return TRUE; }
	void* Create(BOOL /*loading = FALSE*/) override { return new BerconWood(); }
	const TCHAR *	ClassName() override { return GetString(IDS_BERCON_WOOD); }
	SClass_ID SuperClassID() override { return TEXMAP_CLASS_ID; }
	Class_ID ClassID() override { return BerconWood_CLASS_ID; }
	const TCHAR* Category() override { return TEXMAP_CAT_3D; }
	const TCHAR* InternalName() override { return _M("BerconWood"); } // returns fixed parsable name (scripter-visible name)
	HINSTANCE HInstance() override { return hInstance; }
};
