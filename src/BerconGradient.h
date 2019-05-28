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

#include <Windows.h>
#include "BerconCommon.h"
#include "IGradient.h"
#include "chkmtlapi.h"

extern MCHAR *GetString(int id);

extern HINSTANCE hInstance;

//#define GRADIENT3D

class BerconGradient final : public Texmap, public GradientMap, public ResourceMakerCallback {
	public:
		// Parameter block
		IParamBlock2	*pblock;
//		IParamBlock2	*pbCurve; //ref CURVEPB_REF
		IParamBlock2	*pbXYZ; //ref COORDS

		BerconXYZ berconXYZ;
		static ParamDlg* texoutDlg;
		TextureOutput   *texout;

		Interval		ivalid;	

		// Values		
		GradientRamp* gradient;
		IGradient* gradientUI;
		int dirty;
		
		int p_seed;
		int p_type;
		int p_randObj; int p_randMat; int p_randPar; int p_randTile;
		float p_rangeMin; float p_rangeMax;
		int p_rangeLoop;
		int p_uvwType;
		int p_reverse;		
		int p_normalType;
		int p_normalFunction;
		float p_ior;
		int p_disOn;
		float p_disStr;

		// Helpers
		void seedRandomGen(ShadeContext& sc);
		int limitRange(float& d);
		void EnableStuff();		

		// Shading
		float getGradientValueUVW(Point3 p);
		float getGradientValueDist(ShadeContext& sc);
		float getGradientValueNormal(ShadeContext& sc);
		float getGradientValue(ShadeContext& sc);		

		// UI				
		int previewMatIDMode;
		
		// Submaps		
		Texmap* p_maptex;
		Texmap* p_distex;
		INode* p_node;
		
		void setKeyTex(Texmap* m);
		Texmap* getKeyTex();

		// Editing keys
		void keyColorChanged(AColor col);
		void keyNumChanged(int num);
		void keyPosChanged(float pos);
		
		// GradientRampInterface
		int countKeys();
		void resetKeys();
		
		// From GradientMap
		void gradAddKey(float pos) override;
		void gradMoveKey(int n, float pos) override;
		void gradDelKey(int n) override;
		void gradSelKey() override;
		void gradReset() override { resetKeys(); }

		// Curve
	/*
		ICurveCtl* curve;				
		BOOL p_curveOn;
		// From ResourceMakerCallback		
		BOOL SetCustomImageList(HIMAGELIST &hCTools,ICurveCtl *pCCtl) { return TRUE; };
		BOOL GetToolTip(int iButton, TSTR &ToolTip,ICurveCtl *pCCtl) { return TRUE; };
		void ResetCallback(int curvenum, ICurveCtl *pCCtl) { ICurve *pCurve = NULL; pCurve = pCCtl->GetControlCurve(curvenum); if(pCurve) { pCurve->SetNumPts(2); NewCurveCreatedCallback(curvenum, pCCtl); }}
		void NewCurveCreatedCallback(int curvenum, ICurveCtl *pCCtl) {
			ICurve *pCurve = NULL; pCurve = pCCtl->GetControlCurve(curvenum); TimeValue t = GetCOREInterface()->GetTime();
			CurvePoint pt = pCurve->GetPoint(t,0); pt.p.y = 0.f; pCurve->SetPoint(t,0,&pt);
			pCurve->SetPenProperty( RGB(0,0,0)); pCurve->SetDisabledPenProperty( RGB(128,128,128));		
			pt = pCurve->GetPoint(t,1); pt.p.y = 1.f; pCurve->SetPoint(t,1,&pt);
		}
		*/
		//From MtlBase
		ParamDlg* CreateParamDlg(HWND hwMtlEdit, IMtlParams *imp) override;
		BOOL SetDlgThing(ParamDlg* dlg) override;
		void Update(TimeValue t, Interval& valid) override;
		void Reset() override;
		Interval Validity(TimeValue t) override;
		
		ULONG LocalRequirements(int subMtlNum) override { return berconXYZ.req(); }
		void MappingsRequired(int subMtlNum, BitArray& mapreq, BitArray& bumpreq) override { berconXYZ.map(subMtlNum, mapreq, bumpreq); }

		int NumSubTexmaps() override { return 3 + gradient->numKeys(); } // !! Update submap count !!
		Texmap* GetSubTexmap(int i) override;
		void SetSubTexmap(int i, Texmap *m) override;
		TSTR GetSubTexmapSlotName(int i) override;
		
		//From Texmap
		RGBA EvalColor(ShadeContext& sc) override;
		float EvalMono(ShadeContext& sc) override;
		Point3 EvalNormalPerturb(ShadeContext& sc) override;						
		int SubNumToRefNum(int subNum) override { return subNum; }		

		void ReadSXPData(TCHAR *name, void *sxpdata) {}

		// Interactive Display
		TexHandle *texHandle;
		Interval texHandleValid;
		void DiscardTexHandle() { if (texHandle) { texHandle->DeleteThis(); texHandle = NULL; } }
		BOOL SupportTexDisplay() override { return TRUE; }
		void ActivateTexDisplay(BOOL onoff) override { if (!onoff) DiscardTexHandle(); }
		DWORD_PTR GetActiveTexHandle(TimeValue t, TexHandleMaker& thmaker) override;

		//From Animatable
		Class_ID ClassID() override {return BerconGradient_CLASS_ID;}		
		SClass_ID SuperClassID() override { return TEXMAP_CLASS_ID; }
		void GetClassName(TSTR& s) override {s = GetString(IDS_BERCON_COLOR);}

		RefTargetHandle Clone( RemapDir &remap ) override;
		RefResult NotifyRefChanged(NOTIFY_REF_CHANGED_ARGS) override;

		int NumSubs() override { return gradient->numKeys(); } // !! Update submap count !!
		Animatable* SubAnim(int i) override; 
		TSTR SubAnimName(int i) override;

		int NumRefs() override; // !! Update submap count !!		// This was 7+gradient
		RefTargetHandle GetReference(int i) override;
private:
		void SetReference(int i, RefTargetHandle rtarg) override;
public:
		int	NumParamBlocks() override { return 2; }
		IParamBlock2* GetParamBlock(int i) override
		{ switch (i) { case 0: return pblock; /*case 1: return pbCurve;*/ case 2: return pbXYZ; } return NULL; }
		IParamBlock2* GetParamBlockByID(BlockID id) override
		{ 
			if (pblock->ID() == id) return pblock;
		//	if (pbCurve->ID() == id) return pbCurve;
			if (pbXYZ->ID() == id) return pbXYZ;
			return NULL;
		}


		int RenderBegin(TimeValue t, ULONG flags = 0) override
		{ if (!flags) previewMatIDMode = FALSE; return 1; }
		int RenderEnd(TimeValue t) override
		{ previewMatIDMode = TRUE; return 1; }				

		void DeleteThis() override { delete this; }		
				
		BerconGradient();
		virtual ~BerconGradient();		
		
		void* GetInterface(ULONG id) override
		{
		/*	if(id == I_RESMAKER_INTERFACE)
				return (void *) (ResourceMakerCallback*) this;
			else*/
				return Texmap::GetInterface(id);
		}
};

class BerconGradientClassDesc : public ClassDesc2 {
public:
	int IsPublic() override { return TRUE; }
	void* Create(BOOL /*loading = FALSE*/) override { return new BerconGradient(); }
	const TCHAR *	ClassName() override { return GetString(IDS_BERCON_COLOR); }
	SClass_ID SuperClassID() override { return TEXMAP_CLASS_ID; }
	Class_ID ClassID() override { return BerconGradient_CLASS_ID; }
	const TCHAR* Category() override { return TEXMAP_CAT_3D; }
	const TCHAR* InternalName() override { return _M("BerconGradient"); } // scripter-visible name
	HINSTANCE HInstance() override { return hInstance; }
};
