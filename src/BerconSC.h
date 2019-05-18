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

#include <max.h>
#include <imtl.h>

//!! THIS APPEARS TO ONLY BE USED BY DISTORTION AND TILE

#define BERCON_SHADE_CONTEXT_CLASS_ID Class_ID(0x7c0a38f1, 0x2f1a67f2)

class BerconSC : public ShadeContext, public RenderGlobalContext {
private:
	ShadeContext* const sc;			

	int type;
	Point3 uvPoint;
	Point3 uvPoint2;
	int uvChannel;
	int uvChannel2;
	bool useMultiTexture;
	float val;

public:
	BerconSC(ShadeContext* const sc) : sc(sc), val(0)
	{
		mode = sc->mode;												// Line 370 of ShadeContext.h
		doMaps = sc->doMaps;
		filterMaps = sc->filterMaps;
		shadow = sc->shadow;
		backFace = sc->backFace;
		mtlNum = sc->mtlNum;
		ambientLight = sc->ambientLight;
		nLights = sc->nLights;
		rayLevel = sc->rayLevel;
		xshadeID = sc->xshadeID;
		atmosSkipLight = sc->atmosSkipLight;
		globContext = sc->globContext;
		out = sc->out;

		uvChannel = -1;
		uvChannel2 = -1;
		useMultiTexture = false;
		type = -1;
	}

	void setParams(Point3 p, int type, int chan = 0) {				// These next four functions appear to be the only thing
		uvPoint = p;												// actually used anywhere, especially if MultiTexture is not the build target
		this->type = type;											// setParams, setUV1, etc., used in Tile and Distortion
		uvChannel = chan;
	}

	void setUV1(Point3 uvw, int chan) {
		uvPoint = uvw;
		uvChannel = chan;
	}

	void setUV2(Point3 uvw, int chan) {
		uvPoint2 = uvw;
		uvChannel2 = chan;
	}

	void setMultiTexture(float val) {
		this->val = val;
		useMultiTexture = true;
	}

	void ResetOutput(int n) { sc->ResetOutput(n); }
	Class_ID ClassID() override { return BERCON_SHADE_CONTEXT_CLASS_ID; }
	BOOL InMtlEditor() override { return sc->InMtlEditor(); }
	int Antialias() override { return sc->Antialias(); }
	int ProjType() override { return sc->ProjType(); }
	LightDesc* Light(int n) override { return sc->Light(n); }
	TimeValue CurTime() override { return sc->CurTime(); }
	int NodeID() override { return sc->NodeID(); }
	INode* Node() override { return sc->Node(); }
	Object* GetEvalObject() override { return sc->GetEvalObject(); }
	Point3 BarycentricCoords() override { return sc->BarycentricCoords(); }
	int FaceNumber() override { return sc->FaceNumber(); }
	Point3 Normal() override { return sc->Normal(); }
	void SetNormal(Point3 p) override { sc->SetNormal(p); }
	Point3 OrigNormal() override { return sc->OrigNormal(); }
	Point3 GNormal() override { return sc->GNormal(); }
	float Curve() override { return sc->Curve(); }
	Point3 V() override { return sc->V(); }
	void SetView(Point3 p) override { sc->SetView(p); }
	Point3 OrigView() override { return sc->OrigView(); }
	Point3 ReflectVector() override { return sc->ReflectVector(); }
	Point3 RefractVector(float ior) override { return sc->RefractVector(ior); }
	void SetIOR(float ior) override { sc->SetIOR(ior); }
	float GetIOR() override { return sc->GetIOR(); }
	Point3 CamPos() override { return sc->CamPos(); }
	Point3 P() override; // Overridden
	Point3 DP() override { return sc->DP(); }
	void DP(Point3& dpdx, Point3& dpdy) override { sc->DP(dpdx, dpdy); }		// error fixed May 2019
	Point3 PObj() override; // Overridden
	Point3 DPObj() override { return sc->DPObj(); }
	Box3 ObjectBox() override { return sc->ObjectBox(); }
	inline Point3 PObjRelBox() override; // Overridden
	Point3 DPObjRelBox() override { return sc->DPObjRelBox(); }
	inline void ScreenUV(Point2& uv, Point2& duv) override; // Overridden		
	IPoint2 ScreenCoord() override { return sc->ScreenCoord(); }
	Point2 SurfacePtScreen() override { return sc->SurfacePtScreen(); }
	inline Point3 UVW(int channel) override; // Overridden
	Point3 DUVW(int channel) override { return sc->DUVW(channel); }
	void DPdUVW(Point3 dP[3], int channel) override { sc->DPdUVW(dP, channel); }
	int BumpBasisVectors(Point3 dP[2], int axis, int channel) override { return sc->BumpBasisVectors(dP, axis, channel); }
	BOOL IsSuperSampleOn() override { return sc->IsSuperSampleOn(); }
	BOOL IsTextureSuperSampleOn() override { return sc->IsTextureSuperSampleOn(); }
	int GetNSuperSample() override { return sc->GetNSuperSample(); }
	float GetSampleSizeScale() override { return sc->GetSampleSizeScale(); }
	Point3 UVWNormal(int channel) override { return sc->UVWNormal(channel); }
	float RayDiam() override { return sc->RayDiam(); }
	float RayConeAngle() override { return sc->RayConeAngle(); }
	AColor EvalEnvironMap(Texmap* map, Point3 view) override { return sc->EvalEnvironMap(map, view); }
	void GetBGColor(Color& bgcol, Color& transp, BOOL fogBG) override { sc->GetBGColor(bgcol, transp, fogBG); }
	float CamNearRange() override { return sc->CamNearRange(); }
	float CamFarRange() override { return sc->CamFarRange(); }
	Point3 PointTo(const Point3& p, RefFrame ito) override { return sc->PointTo(p, ito); }
	Point3 PointFrom(const Point3& p, RefFrame ifrom) override { return sc->PointFrom(p, ifrom); }
	Point3 VectorTo(const Point3& p, RefFrame ito) override { return sc->VectorTo(p, ito); }
	Point3 VectorFrom(const Point3& p, RefFrame ifrom) override { return sc->VectorFrom(p, ifrom); }
	Point3 VectorToNoScale(const Point3& p, RefFrame ito) override { return sc->VectorToNoScale(p, ito); }
	Point3 VectorFromNoScale(const Point3& p, RefFrame ifrom) override { return sc->VectorFromNoScale(p, ifrom); }
	void SetGBufferID(int gbid) override { sc->SetGBufferID(gbid); }
	FILE* DebugFile() override { return 0; }																//	This is only used internally by ShadeContext anyway. But virtualize this because SC is virtual.
	AColor EvalGlobalEnvironMap(Point3 dir) override { return sc->EvalGlobalEnvironMap(dir); }
	BOOL GetCache(Texmap* map, AColor& c) override { return sc->GetCache(map, c); }
	BOOL GetCache(Texmap* map, float& f) override { return sc->GetCache(map, f); }
	BOOL GetCache(Texmap* map, Point3& p) override { return sc->GetCache(map, p); }
	void PutCache(Texmap* map, const AColor& c) override { sc->PutCache(map, c); }
	void PutCache(Texmap* map, const float f) override { sc->PutCache(map, f); }
	void PutCache(Texmap* map, const Point3& p) override { sc->PutCache(map, p); }
	void TossCache(Texmap* map) override { sc->TossCache(map); }

	INT_PTR Execute(int cmd, ULONG_PTR arg1, ULONG_PTR arg2, ULONG_PTR arg3) override
	{
		return sc->Execute(cmd, arg1, arg2, arg3);
	}

	LightDesc* GetAtmosSkipLight() { return sc->GetAtmosSkipLight(); }
	void SetAtmosSkipLight(LightDesc* lt) { sc->SetAtmosSkipLight(lt); }
	int NRenderElements() override { return sc->NRenderElements(); }
	IRenderElement* GetRenderElement(int n) override { return sc->GetRenderElement(n); }
	Color DiffuseIllum() override { return sc->DiffuseIllum(); }

	// REVISED METHODS OF CALLING GLOBCONTEXT AND TONEOPERATOR FOR THESE FUNCTIONS

	
		virtual ToneOperator* IsPhysicalSpace() const
		{
			if (globContext != NULL && globContext->pToneOp !=NULL) return sc->globContext->pToneOp;			// Always check globContext and pToneOp for null
			else return NULL;
		}

			virtual ToneOperator* ScaledToRGB(float /*energy*/) const
		{
			if (globContext != NULL && globContext->pToneOp != NULL) return sc->globContext->pToneOp;
			else return NULL;
		}

		virtual ToneOperator* ScaledToRGB() const
		{
			if (globContext != NULL && globContext->pToneOp != NULL) return sc->globContext->pToneOp;
			else return NULL;
		}

		ToneOperator* ScalePhysical(float /*energy*/) const
		{
			if (globContext != NULL && globContext->pToneOp != NULL) return sc->globContext->pToneOp;

			else return NULL;
		}


		virtual ToneOperator* ScaleRGB(float /*energy*/) const
		{
			if (globContext != NULL && globContext->pToneOp != NULL) return sc->globContext->pToneOp;
			else return NULL;
		}
	




	/*	THIS SECTION IS DEPRECATED AND WILL THROW COMPILER ERROR C4996
	bool IsPhysicalSpace () const{ return sc->IsPhysicalSpace(); }
	template<class T> void ScaledToRGB (T& energy) const { sc->ScaledToRGB(energy); }
	float ScaledToRGB (float energy) const{ return sc->ScaledToRGB(energy); }
	void ScaledToRGB (){ sc->ScaledToRGB(); }
	template<class T> void ScalePhysical (T& energy) const { sc->ScaledPhysical(energy); }
	float ScalePhysical (float energy) const{ return sc->ScalePhysical(energy); }
	template<class T> void ScaleRGB (T& energy) const { sc->ScaleRGB(energy); }
	float ScaleRGB (float energy) const{ return sc->ScaleRGB(energy); }
};
	*/
};
	inline Point3 BerconSC::PObjRelBox() {
	return sc->PObjRelBox();
}

	inline void BerconSC::ScreenUV(Point2& uv, Point2 &duv) {
	sc->ScreenUV(uv, duv);
	if (type == 4) {
		uv.x = uvPoint.x;
		uv.y = uvPoint.y;
	}
}

inline Point3 BerconSC::P() {
	return type == 3 ? uvPoint : sc->P();
}

inline Point3 BerconSC::PObj() {
	return type == 2 ? uvPoint : sc->PObj();	
}

inline Point3 BerconSC::UVW(int channel) {
	if (useMultiTexture && 99 == channel)
		return Point3(val, val, val);

	if ((type == 0 || type == 1) && uvChannel == channel)
		return uvPoint;

	if (uvChannel > 0 && uvChannel == channel)
		return uvPoint;
	
	if (uvChannel2 > 0 && uvChannel2 == channel)
		return uvPoint2;
	
	return sc->UVW(channel);
}
