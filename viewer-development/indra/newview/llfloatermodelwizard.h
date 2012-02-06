/** 
 * @file llfloatermodelwizard.h
 *
 * $LicenseInfo:firstyear=2009&license=viewerlgpl$
 * GIS Viewer Source Code
 * Copyright (C) 2010, Linden Research, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation;
 * version 2.1 of the License only.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 * Linden Research, Inc., 945 Battery Street, San Francisco, CA  94111  USA
 * $/LicenseInfo$
 */

#ifndef LLFLOATERMODELWIZARD_H
#define LLFLOATERMODELWIZARD_H


#include "llmeshrepository.h"
#include "llmodel.h"
#include "llthread.h"
#include "llfloatermodeluploadbase.h"


class LLModelPreview;


class LLFloaterModelWizard : public LLFloaterModelUploadBase
{
public:
	
	class DecompRequest : public LLPhysicsDecomp::Request
	{
	public:
		S32 mContinue;
		LLPointer<LLModel> mModel;
		
		DecompRequest(const std::string& stage, LLModel* mdl);
		virtual S32 statusCallback(const char* status, S32 p1, S32 p2);
		virtual void completed();
		
	};
	
	static LLFloaterModelWizard* sInstance;

	LLFloaterModelWizard(const LLSD& key);
	virtual ~LLFloaterModelWizard();
	/*virtual*/	BOOL	postBuild();
	void			draw();
	void            refresh();
	
	BOOL handleMouseDown(S32 x, S32 y, MASK mask);
	BOOL handleMouseUp(S32 x, S32 y, MASK mask);
	BOOL handleHover(S32 x, S32 y, MASK mask);
	BOOL handleScrollWheel(S32 x, S32 y, S32 clicks);

	void setDetails(F32 x, F32 y, F32 z, F32 streaming_cost, F32 physics_cost);
	void modelLoadedCallback();
	void modelChangedCallback();
	void initDecompControls();
	
	// shows warning message if agent has no permissions to upload model
	/*virtual*/ void onPermissionsReceived(const LLSD& result);

	// called when error occurs during permissions request
	/*virtual*/ void setPermissonsErrorStatus(U32 status, const std::string& reason);

	/*virtual*/ void onModelPhysicsFeeReceived(const LLSD& result, std::string upload_url);

	/*virtual*/ void setModelPhysicsFeeErrorStatus(U32 status, const std::string& reason);

	/*virtual*/ void onModelUploadSuccess();

	/*virtual*/ void onModelUploadFailure();

	const LLRect& getPreviewRect() const { return mPreviewRect; }

	LLPhysicsDecomp::decomp_params mDecompParams;
	std::set<LLPointer<DecompRequest> > mCurRequest;
	std::string mStatusMessage;
	static void executePhysicsStage(std::string stage_name);

private:
	enum EWizardState
	{
		CHOOSE_FILE = 0,
		OPTIMIZE,
		PHYSICS,
		REVIEW,
		UPLOAD
	};

	void setState(int state);
	void updateButtons();
	void onClickSwitchToAdvanced();
	void onClickRecalculateGeometry();
	void onClickRecalculatePhysics();
	void onClickCalculateUploadFee();
	void onClickCancel();
	void onClickBack();
	void onClickNext();
	bool onEnableNext();
	bool onEnableBack();
	void loadModel();
	void onPreviewLODCommit(LLUICtrl*);
	void onUpload();

	LLModelPreview*	mModelPreview;
	LLRect			mPreviewRect;
	int mState;

	S32				mLastMouseX;
	S32				mLastMouseY;

	U32			    mLastEnabledState;

	LLButton*		mRecalculateGeometryBtn;
	LLButton*		mRecalculatePhysicsBtn;
	LLButton*		mRecalculatingPhysicsBtn;
	LLButton*		mCalculateWeightsBtn;
	LLButton*		mCalculatingWeightsBtn;

	LLView*		mChooseFilePreviewPanel;
	LLView*		mOptimizePreviewPanel;
	LLView*		mPhysicsPreviewPanel;
};


#endif
