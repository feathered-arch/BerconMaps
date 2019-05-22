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

/*
	This is the general include file, used by all maps.
*/

#pragma once
#include <immintrin.h>
#include "BerconCommon.h"

extern MCHAR* GetString(int id);

extern HINSTANCE hInstance;


#define NOISE_NSUBTEX		18  // Number of subtextures

class BerconNoise; // why is this here?

class BerconNoise : public Texmap, public ResourceMakerCallback/*, public imrShaderTranslation*/
{
public:

	RefResult NotifyRefChanged(NOTIFY_REF_CHANGED_ARGS) override;

	bool mappedParameters{};
	NoiseParams EvalParameters(ShadeContext* sc);

	//void limitLevel(Point3 dp, NoiseParams &np, float nSize);

	//void calcAverage();
	//float average;

	// Noise variables	
	float size{};
	float phase{};
	float spread{};
	float F1{}, F2{}, F3{}, F4{};

	int noiseFunction{};
	int worleyFunction{};
	int fractalFunction{};

	// Fractal variables
	float levels{};
	float low{}, high{};
	float fractalH{}, fractalOffset{}, fractalGain{}, fractalLacunarity{};

	// Distortion		
	BOOL useDistortion{};
	float distortionStr{};
	void applyDistortion(ShadeContext& sc, Point3& p);
	int uvwDist{};

	// User Interface
	void EnableStuff();

	// Parameter block
	IParamBlock2* pbXYZ;
	IParamBlock2* pblock; //ref 0
	//		IParamBlock2	*pbCurve;	//ref CURVEPB_REF
	IParamBlock2* pbMap; //ref PBMAP_REF

	BerconXYZ berconXYZ;

	Color col[2];
	Texmap* subtex[NOISE_NSUBTEX]{}; //array of sub-materials
	BOOL mapOn[NOISE_NSUBTEX]{};
	static ParamDlg* texoutDlg;
	TextureOutput* texout;
	Interval ivalid;

	// Curve
	//		ICurveCtl* curve;				
	//		BOOL useCurve{};
	// From ResourceMakerCallback		
	//		BOOL SetCustomImageList(HIMAGELIST &hCTools,ICurveCtl *pCCtl) { return TRUE; };
	//		BOOL GetToolTip(int iButton, TSTR &ToolTip,ICurveCtl *pCCtl) { return TRUE; };

	/*		virtual void NewCurveCreatedCallback(int curvenum, ICurveCtl* pCCtl)
			{
				ICurve* pCurve = pCCtl->GetControlCurve(curvenum);
				TimeValue t = GetCOREInterface()->GetTime();
				CurvePoint pt = pCurve->GetPoint(t, 0);
				pt.p.y = 0.f;
				pCurve->SetPoint(t, 0, &pt);
				pCurve->SetPenProperty(RGB(0, 0, 0));
				pCurve->SetDisabledPenProperty(RGB(128, 128, 128));
				pt = pCurve->GetPoint(t, 1);
				pt.p.y = 1.f;
				pCurve->SetPoint(t, 1, &pt);
			}
	
			virtual void ResetCallback(int curvenum, ICurveCtl* pCCtl)
			{
				ICurve* pCurve = pCCtl->GetControlCurve(curvenum);
				if (pCurve)
				{
					pCurve->SetNumPts(2);
					NewCurveCreatedCallback(curvenum, pCCtl);
				}
			}
	*/

	// Interactive Display
	TexHandle* texHandle;
	Interval texHandleValid;

	void DiscardTexHandle()
	{
		if (texHandle)
		{
			texHandle->DeleteThis();
			texHandle = NULL;
		}
	}

	BOOL SupportTexDisplay() override { return TRUE; }
	void ActivateTexDisplay(BOOL onoff) override { if (!onoff) DiscardTexHandle(); }
	DWORD_PTR GetActiveTexHandle(TimeValue t, TexHandleMaker& thmaker) override;

	//From MtlBase
	ParamDlg* CreateParamDlg(HWND hwMtlEdit, IMtlParams* imp) override;
	BOOL SetDlgThing(ParamDlg* dlg) override;
	void Update(TimeValue t, Interval& valid) override;
	void Reset() override;
	Interval Validity(TimeValue t) override;
	ULONG LocalRequirements(int subMtlNum) override { return berconXYZ.req(); }

	void MappingsRequired(int subMtlNum, BitArray& mapreq, BitArray& bumpreq) override
	{
		berconXYZ.map(subMtlNum, mapreq, bumpreq);
	}


	int NumSubTexmaps() override { return NOISE_NSUBTEX; }
	Texmap* GetSubTexmap(int i) override { return subtex[i]; }
	void SetSubTexmap(int i, Texmap* m) override;
	TSTR GetSubTexmapSlotName(int i) override;

	//From Texmap
	RGBA EvalColor(ShadeContext& sc) override;
	float EvalMono(ShadeContext& sc) override;
	Point3 EvalNormalPerturb(ShadeContext& sc) override;

	XYZGen* GetTheXYZGen() override { return NULL; }

	int SubNumToRefNum(int subNum) override { return subNum; }

	void ReadSXPData(MCHAR *name, void *sxpdata) { }

	//From Animatable
	Class_ID ClassID() override { return BerconNoise_CLASS_ID; }
	SClass_ID SuperClassID() override { return TEXMAP_CLASS_ID; }
	void GetClassName(TSTR& s) override { s = GetString(IDS_CLASS_NAME); }

private:
	void SetReference(int i, RefTargetHandle rtarg) override;
public:
	RefTargetHandle GetReference(int i) override;

	RefTargetHandle Clone(RemapDir& remap) override;

	int NumSubs() override { return 1; }	// Set this to one when Paramblock is used as per max sdk
	Animatable* SubAnim(int i) override;
	TSTR SubAnimName(int i) override;

	// TODO: Maintain the number or references here 
	int NumRefs() override { return 22; }	// Was 24; minus two for custom curve being gone


	int NumParamBlocks() override { return 3; }

	IParamBlock2* GetParamBlock(int i) override
	{
		switch (i)
		{
		case 0: return pblock; /*case 1: return pbCurve;*/
		case 1: return pbMap;
		case 2: return pbXYZ;
		}
		return NULL;
	}

	IParamBlock2* GetParamBlockByID(BlockID id) override
	{
		if (pblock->ID() == id) return pblock;
		//		if (pbCurve->ID() == id) return pbCurve;
		if (pbMap->ID() == id) return pbMap;
		if (pbXYZ->ID() == id) return pbXYZ;
		return NULL;
	}

	//Constructor/Destructor
	BerconNoise();
	virtual ~BerconNoise();

	void DeleteThis() override { delete this; }

	void* GetInterface(ULONG id) {
		/*	if(id == I_RESMAKER_INTERFACE)
				return (void *) (ResourceMakerCallback*) this;
			else*/
		return Texmap::GetInterface(id);
	}
};

class BerconNoiseClassDesc : public ClassDesc2
{
public:
	int IsPublic() override					{ return TRUE; }
	void* Create(BOOL) override				{ return new BerconNoise(); }
	const MCHAR* ClassName() override		{ return GetString(IDS_CLASS_NAME); }
	SClass_ID SuperClassID() override		{ return TEXMAP_CLASS_ID; }
	Class_ID ClassID() override				{ return BerconNoise_CLASS_ID; }
	const MCHAR* Category() override		{ return TEXMAP_CAT_3D; }
	const MCHAR* InternalName() override	{ return _M("BerconNoise"); }
	// returns fixed parsable name (scripter-visible name)
	HINSTANCE HInstance() override			{ return hInstance; }
};
