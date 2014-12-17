// RoundRect.cpp: implementation of the CRoundRect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RoundRect.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CRoundRect,CElement)

CRoundRect::CRoundRect()
{
	m_bHavePen=true;
	m_bHaveBrush=true;
	m_bHaveFont=false;

	m_fw=(float)0.5;
	m_fh=(float)0.5;
}

CRoundRect::~CRoundRect()
{

}

void CRoundRect::Draw(CDC *pDC)
{
	long x1,y1,x2,y2;
	long x3,y3;

	if(m_DnyAttr.m_Linked && m_DnyAttr.m_na){
		CPen *pop=(CPen*)(pDC->SelectStockObject(BLACK_PEN));
		CBrush *pob=(CBrush*)(pDC->SelectStockObject(WHITE_BRUSH));
		UPtoLP(m_lox1,m_loy1,&x1,&y1);
		UPtoLP(m_lox2,m_loy2,&x2,&y2);
		pDC->Rectangle(x1,y1,x2,y2);
		pDC->SelectObject(pop);
		pDC->SelectObject(pob);
		pDC->DrawIcon(x1,y1,ghNA);
		return;
	}

	if(m_DnyAttr.m_Linked && !m_DnyAttr.m_vi) return;

	LOGPEN tPen=m_pen;
	UPtoLP(m_pen.lopnWidth.x,m_pen.lopnWidth.y,&tPen.lopnWidth.x,&tPen.lopnWidth.y);
	CPen pen;
	pen.CreatePenIndirect(&tPen);
	CBrush brush;
	brush.CreateBrushIndirect(&m_brush);
	pDC->SetBkMode(TRANSPARENT);

	CPen *pOldPen;
	CBrush *pOldBrush;
	UPtoLP(m_lx1,m_ly1,&x1,&y1);
	UPtoLP(m_lx2,m_ly2,&x2,&y2);
	UPtoLP((long)(abs(m_lx2-m_lx1)*(float)m_fw),(long)(abs(m_ly2-m_ly1)*(float)m_fh),&x3,&y3);
	if(m_DnyAttr.m_FillH.m_Used || m_DnyAttr.m_FillV.m_Used){
		long tx1=x1;
		long ty1=y1;
		long tx2=x2;
		long ty2=y2;
		float fp;
		if(m_DnyAttr.m_FillH.m_Used){
			fp=(getFloat(m_DnyAttr.m_FillH.m_TagValue)-m_DnyAttr.m_FillH.m_LTO)*\
				(m_DnyAttr.m_FillH.m_RBT-m_DnyAttr.m_FillH.m_LTT)/\
				(m_DnyAttr.m_FillH.m_RBO-m_DnyAttr.m_FillH.m_LTO)/100;
            fp += m_DnyAttr.m_FillH.m_LTT/100;
			if(fp<0.) fp=0.;
			if(fp>1.) fp=1.;
			if(m_DnyAttr.m_FillH.m_Ref==0){//to left
				tx1=tx2-(tx2-tx1)*fp;
			}else{//to right
				tx2=tx1+(tx2-tx1)*fp;
			}
		}
		if(m_DnyAttr.m_FillV.m_Used){
			fp=(getFloat(m_DnyAttr.m_FillV.m_TagValue)-m_DnyAttr.m_FillV.m_LTO)*\
				(m_DnyAttr.m_FillV.m_RBT-m_DnyAttr.m_FillV.m_LTT)/\
				(m_DnyAttr.m_FillV.m_RBO-m_DnyAttr.m_FillV.m_LTO)/100;
            fp += m_DnyAttr.m_FillV.m_LTT/100;
			if(fp<0.) fp=0.;
			if(fp>1.) fp=1.;
			if(m_DnyAttr.m_FillV.m_Ref==0){//to top
				ty1=ty2-(ty2-ty1)*fp;
			}else{//to bottom
				ty2=ty1+(ty2-ty1)*fp;
			}
		}
		CRgn rgn1;
		rgn1.CreateRoundRectRgn(x1,y1,x2,y2,x3,y3);
		CRgn rgn2;
		rgn2.CreateRectRgn(tx1,ty1,tx2,ty2);
		CRgn rgn3;
		rgn3.CreateRectRgn(x1,y1,x2,y2);
		rgn3.CombineRgn(&rgn2,&rgn1,RGN_AND);
		pOldBrush=pDC->SelectObject(&brush);
		pOldPen=(CPen*)(pDC->SelectStockObject(NULL_PEN));
		pDC->PaintRgn(&rgn3);
		pDC->SelectStockObject(NULL_BRUSH);
		pDC->SelectObject(&pen);
		pDC->RoundRect(x1,y1,x2,y2,x3,y3);
	}else{
		pOldBrush=pDC->SelectObject(&brush);
		pOldPen=pDC->SelectObject(&pen);
		pDC->RoundRect(x1,y1,x2,y2,x3,y3);
	}

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

void CRoundRect::DrawTracker(CDC *pDC)
{
	LOGPEN tLogPen;
	tLogPen.lopnColor=RGB(0,0,0);
	tLogPen.lopnStyle=PS_DASH;
	tLogPen.lopnWidth.x=1;
	CPen tPen;
	tPen.CreatePenIndirect(&tLogPen);
	CPen *pOldPen=(CPen *)pDC->SelectObject(&tPen);
	CBrush *pOldBrush1;
	CBrush *pOldBrush=(CBrush *)pDC->SelectStockObject(NULL_BRUSH);
	pDC->SetROP2(R2_NOTXORPEN);
	long x1,y1,x2,y2,x3,y3;
	int lx3,ly3;

	UPtoLP(m_lx1,m_ly1,&x1,&y1);
	UPtoLP(m_lx2,m_ly2,&x2,&y2);
	UPtoLP((long)(abs(m_lx2-m_lx1)*(float)m_fw),(long)(abs(m_ly2-m_ly1)*(float)m_fh),&x3,&y3);
	pDC->Rectangle(x1,y1,x2,y2);
	switch(m_lHitTarget){
	case TK_LT:
		m_lx1=m_potTracker.x;
		m_ly1=m_potTracker.y;
		UPtoLP(m_lx1,m_ly1,&x1,&y1);
		UPtoLP((long)(abs(m_lx2-m_lx1)*(float)m_fw),(long)(abs(m_ly2-m_ly1)*(float)m_fh),&x3,&y3);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_LB:
		m_lx1=m_potTracker.x;
		m_ly2=m_potTracker.y;
		UPtoLP(m_lx1,m_ly2,&x1,&y2);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_RT:
		m_lx2=m_potTracker.x;
		m_ly1=m_potTracker.y;
		UPtoLP(m_lx2,m_ly1,&x2,&y1);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_RB:
		m_lx2=m_potTracker.x;
		m_ly2=m_potTracker.y;
		UPtoLP(m_lx2,m_ly2,&x2,&y2);
		UPtoLP((long)(abs(m_lx2-m_lx1)*(float)m_fw),(long)(abs(m_ly2-m_ly1)*(float)m_fh),&x3,&y3);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_LM:
		m_lx1=m_potTracker.x;
		UPtoLP(m_lx1,m_lx1,&x1,0);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_TM:
		m_ly1=m_potTracker.y;
		UPtoLP(m_ly1,m_ly1,0,&y1);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_RM:
		m_lx2=m_potTracker.x;
		UPtoLP(m_lx2,m_lx2,&x2,0);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_BM:
		m_ly2=m_potTracker.y;
		UPtoLP(m_ly2,m_ly2,0,&y2);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	case TK_SP:
		lx3=m_potTracker.x;
		ly3=m_potTracker.y;
		if(lx3<m_lx1) lx3=m_lx1+1;
		if(lx3>m_lx2) lx3=m_lx2-1;
		if(ly3<m_ly1) ly3=m_ly1+1;
		if(ly3>m_ly2) ly3=m_ly2-1;
		m_fw=(float)(lx3-m_lx1)/(float)(m_lx2-m_lx1);
		m_fh=(float)(ly3-m_ly1)/(float)(m_ly2-m_ly1);
		long lDeltaX,lDeltaY;
		UPtoLP((long)(1.2*DELTA),(long)(1.2*DELTA),&lDeltaX,&lDeltaY);
		pOldBrush1=(CBrush*)pDC->SelectStockObject(HOLLOW_BRUSH);
		//pDC->Ellipse(lx3-lDeltaX,ly3-lDeltaY,lx3+lDeltaX,ly3+lDeltaY); 
		pDC->RoundRect(x1,y1,x2,y2,x3,y3);
		pDC->RoundRect(x1,y1,x2,y2,lx3-x1,ly3-y1);
		pDC->SelectObject(pOldBrush1);
		break;
	case TK_MOV:
		m_lx1+=m_potTracker.x;
		m_ly1+=m_potTracker.y;
		m_lx2+=m_potTracker.x;
		m_ly2+=m_potTracker.y;
		UPtoLP(m_lx1,m_ly1,&x1,&y1);
		UPtoLP(m_lx2,m_ly2,&x2,&y2);
		pDC->Rectangle(x1,y1,x2,y2);
		break;
	}

	pDC->SetROP2(R2_COPYPEN);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

void CRoundRect::DrawHotPoints(CDC *pDC)
{
	CBrush *pOldBrush=(CBrush *)pDC->SelectStockObject(GRAY_BRUSH);
	CPen *pOldPen=(CPen *)pDC->SelectStockObject(NULL_PEN);
	pDC->SetROP2(R2_NOT);

	long x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,x6,y6,x7,y7,x8,y8;
	UPtoLP(m_lx1,m_ly1,&x1,&y1);
	UPtoLP(m_lx1,m_ly2,&x2,&y2);
	UPtoLP(m_lx2,m_ly1,&x3,&y3);
	UPtoLP(m_lx2,m_ly2,&x4,&y4);
	UPtoLP((m_lx1+m_lx2)/2,m_ly1,&x5,&y5);
	UPtoLP((m_lx1+m_lx2)/2,m_ly2,&x6,&y6);
	UPtoLP(m_lx1,(m_ly1+m_ly2)/2,&x7,&y7);
	UPtoLP(m_lx2,(m_ly1+m_ly2)/2,&x8,&y8);
	DrawHotPoint(pDC,x1,y1);
	DrawHotPoint(pDC,x2,y2);
	DrawHotPoint(pDC,x3,y3);
	DrawHotPoint(pDC,x4,y4);
	DrawHotPoint(pDC,x5,y5);
	DrawHotPoint(pDC,x6,y6);
	DrawHotPoint(pDC,x7,y7);
	DrawHotPoint(pDC,x8,y8);

	UPtoLP((long)(abs(m_lx2-m_lx1)*(float)m_fw),(long)(abs(m_ly2-m_ly1)*(float)m_fh),&x3,&y3);
	long x0,y0;
	x0=x1+x3;
	y0=y1+y3;
	long lDeltaX,lDeltaY;
	UPtoLP((long)(1.2*DELTA),(long)(1.2*DELTA),&lDeltaX,&lDeltaY);
	pDC->Ellipse(x0-lDeltaX,y0-lDeltaY,x0+lDeltaX,y0+lDeltaY); 

	pDC->SetROP2(R2_COPYPEN);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);	
	pDC->SelectObject(pOldBrush);

}

long CRoundRect::HitTest(long x, long y)
{
    if(!m_DnyAttr.m_Linked) return TK_NONE;

	CRect rtTemp;
	POINT pt;
	pt.x=x;
	pt.y=y;

	int lx3,ly3;
	lx3=m_lx1+abs(m_lx2-m_lx1)*(float)m_fw;
	ly3=m_ly1+abs(m_ly2-m_ly1)*(float)m_fh;
	
	CRgn rgnTmp;
	if(rgnTmp.CreateRoundRectRgn(m_lx1,m_ly1,m_lx2,m_ly2,abs(m_lx2-m_lx1)*(float)m_fw,abs(m_ly2-m_ly1)*(float)m_fh)){
		if(rgnTmp.PtInRegion(pt)) return TK_PUSH;
	}

	return TK_NONE;
}

bool CRoundRect::RInR(LPCRECT lpRect)
{
	if(m_lStatus!=ST_NOMAL) return false;

	CRgn rgnTmp;
	if(!rgnTmp.CreateRoundRectRgn(m_lx1,m_ly1,m_lx2,m_ly2,abs(m_lx2-m_lx1)*(float)m_fw,abs(m_ly2-m_ly1)*(float)m_fh))
        return false;

	if(rgnTmp.RectInRegion(lpRect))
		return true;
	else
		return false;
}

long CRoundRect::Left()
{
	return m_lx1;
}

long CRoundRect::Right()
{
	return m_lx2;
}

long CRoundRect::Top()
{
	return m_ly1;
}

long CRoundRect::Bottom()
{
	return m_ly2;
}

void CRoundRect::Format(long left, long top, long right, long bottom, long flag)
{
	long ldx,ldy;
	float fTemp;
	switch(flag){
	case FM_A2L:
		ldx=left-Left();
		m_lx1+=ldx;
		m_lx2+=ldx;
		break;
	case FM_A2C:
		ldx=(left+right-Left()-Right())/2;
		m_lx1+=ldx;
		m_lx2+=ldx;	
		break;
	case FM_A2R:
		ldx=right-Right();
		m_lx1+=ldx;
		m_lx2+=ldx;
		break;
	case FM_A2T:
		ldy=top-Top();
		m_ly1+=ldy;
		m_ly2+=ldy;
		break;
	case FM_A2M:
		ldy=(top+bottom-Top()-Bottom())/2;
		m_ly1+=ldy;
		m_ly2+=ldy;
		break;
	case FM_A2B:
		ldy=bottom-Bottom();
		m_ly1+=ldy;
		m_ly2+=ldy;
		break;
	case FM_A2CM:
		ldx=(left+right-Left()-Right())/2;
		m_lx1+=ldx;
		m_lx2+=ldx;
		ldy=(top+bottom-Top()-Bottom())/2;
		m_ly1+=ldy;
		m_ly2+=ldy;
		break;
	case FM_R2W:
		ldx=(left+right)/2;
		ldy=(top+bottom)/2;
		R2W(ldx,ldy,&m_lx1,&m_ly1);
		R2W(ldx,ldy,&m_lx2,&m_ly2);
		fTemp=m_fh;
		m_fh=m_fw;
		m_fw=fTemp;
		break;
	case FM_R2U:
		ldx=(left+right)/2;
		ldy=(top+bottom)/2;
		R2U(ldx,ldy,&m_lx1,&m_ly1);
		R2U(ldx,ldy,&m_lx2,&m_ly2);
		fTemp=m_fh;
		m_fh=m_fw;
		m_fw=fTemp;
		break;
	case FM_F2Z:
		ldx=(left+right)/2;
		F2Z(ldx,&m_lx1);
		F2Z(ldx,&m_lx2);
		break;
	case FM_F2I:
		ldy=(top+bottom)/2;
		F2I(ldy,&m_ly1);
		F2I(ldy,&m_ly2);
		break;
	case FM_S2G:
		break;
	}
}

CElement * CRoundRect::Clone(long lCmdId, long lStatus)
{
	CRoundRect * pNew=new CRoundRect();
	pNew->m_lCmdId=lCmdId;
	pNew->m_lStatus=lStatus;
	pNew->m_lHitTarget=m_lHitTarget;
	pNew->m_pFather=this;
	m_pChild=pNew;
	
	pNew->m_lx1=m_lx1;
	pNew->m_lx2=m_lx2;
	pNew->m_ly1=m_ly1;
	pNew->m_ly2=m_ly2;
	pNew->m_fw=m_fw;
	pNew->m_fh=m_fh;
	pNew->m_pen=m_pen;
	pNew->m_brush=m_brush;
	pNew->m_DnyAttr=m_DnyAttr;

	return (CElement*) pNew;
}

void CRoundRect::Load(IXMLDOMElementPtr pGO)
{
    IXMLDOMElementPtr pStatic;
    IXMLDOMElementPtr pDynamic;
	IXMLDOMElementPtr pAttr;
    _variant_t val;

    pStatic=pGO->selectSingleNode(L"./static");
    //position
    pAttr=pStatic->selectSingleNode(L"./position");
    //x1
    val=pAttr->getAttribute(L"x1");
    val.ChangeType(VT_I4);
    m_lx1=val.lVal;
	m_lox1=m_lx1;
    //y1
    val=pAttr->getAttribute(L"y1");
    val.ChangeType(VT_I4);
    m_ly1=val.lVal;
	m_loy1=m_ly1;
    //x2
    val=pAttr->getAttribute(L"x2");
    val.ChangeType(VT_I4);
    m_lx2=val.lVal;
	m_lox2=m_lx2;
    //y2
    val=pAttr->getAttribute(L"y2");
    val.ChangeType(VT_I4);
    m_ly2=val.lVal;
	m_loy2=m_ly2;
    //fw
    val=pAttr->getAttribute(L"fw");
    val.ChangeType(VT_R4);
    m_fw=val.fltVal;
    //fh
    val=pAttr->getAttribute(L"fh");
    val.ChangeType(VT_R4);
    m_fh=val.fltVal;

    //pen
    pAttr=pStatic->selectSingleNode(L"./pen");
    //style
    val=pAttr->getAttribute(L"s");
    val.ChangeType(VT_UI4);
    m_pen.lopnStyle=val.ulVal;
    //width
    val=pAttr->getAttribute(L"w");
    val.ChangeType(VT_I4);
    m_pen.lopnWidth.x=val.lVal;
    //color
    val=pAttr->getAttribute(L"c");
    val.ChangeType(VT_UI4);
    m_pen.lopnColor=val.ulVal;
	m_open=m_pen;

    //brush
    pAttr=pStatic->selectSingleNode(L"./brush");
    //style
    val=pAttr->getAttribute(L"s");
    val.ChangeType(VT_UI4);
    m_brush.lbStyle=val.ulVal;
    //color
    val=pAttr->getAttribute(L"c");
    val.ChangeType(VT_UI4);
    m_brush.lbColor=val.ulVal;
    //hatch
    val=pAttr->getAttribute(L"w");
    val.ChangeType(VT_I4);
    m_brush.lbHatch=val.lVal;
	m_obrush=m_brush;

    //load dynamic attribution if have
    pDynamic=pGO->selectSingleNode(L"./dynamic");
    val=pDynamic->getAttribute(L"linked");
    val.ChangeType(VT_BOOL);
    if(val.boolVal){
        m_DnyAttr.Load(pDynamic);
        m_DnyAttr.m_Linked=true;
    }else{
        m_DnyAttr.m_Linked=false;
    }
}

void CRoundRect::Save(IXMLDOMElementPtr pRoot)
{
	IXMLDOMElementPtr pGO;
	IXMLDOMElementPtr pStatic;
	IXMLDOMElementPtr pDynamic;
	IXMLDOMElementPtr pAttr;
    _variant_t val;

	IXMLDOMDocumentPtr pDoc=pRoot->ownerDocument;
    
    //add a graphy object
    pGO=pDoc->createElement(L"go");    
    val.vt=VT_BSTR;
    val.bstrVal=::SysAllocString(L"roundrect");
    pGO->setAttribute(L"type",val);
    pRoot->appendChild(pGO);

    //add a group static attribution
    pStatic=pDoc->createElement(L"static");
    pGO->appendChild(pStatic);
    
    //position
    pAttr=pDoc->createElement(L"position");
    pStatic->appendChild(pAttr);
    //x1
    val.vt=VT_I4;
    val.lVal=m_lx1;
    pAttr->setAttribute(L"x1",val);
    //y1
    val.vt=VT_I4;
    val.lVal=m_ly1;
    pAttr->setAttribute(L"y1",val);
    //x2
    val.vt=VT_I4;
    val.lVal=m_lx2;
    pAttr->setAttribute(L"x2",val);
    //y2
    val.vt=VT_I4;
    val.lVal=m_ly2;
    pAttr->setAttribute(L"y2",val);
    //fw
    val.vt=VT_R4;
    val.fltVal=m_fw;
    pAttr->setAttribute(L"fw",val);
    //fh
    val.vt=VT_R4;
    val.fltVal=m_fh;
    pAttr->setAttribute(L"fh",val);

    //pen
    pAttr=pDoc->createElement("pen");
    pStatic->appendChild(pAttr);
    //style
    val.vt=VT_UI4;
    val.ulVal=m_pen.lopnStyle;
    pAttr->setAttribute(L"s",val);
    //width
    val.vt=VT_I4;
    val.lVal=m_pen.lopnWidth.x;
    pAttr->setAttribute(L"w",val);
    //color
    val.vt=VT_UI4;
    val.ulVal=m_pen.lopnColor;
    pAttr->setAttribute(L"c",val);

    //brush
    pAttr=pDoc->createElement("brush");
    pStatic->appendChild(pAttr);
    //style
    val.vt=VT_UI4;
    val.ulVal=m_brush.lbStyle;
    pAttr->setAttribute(L"s",val);
    //color
    val.vt=VT_UI4;
    val.ulVal=m_brush.lbColor;
    pAttr->setAttribute(L"c",val);
    //hatch
    val.vt=VT_I4;
    val.lVal=m_brush.lbStyle;
    pAttr->setAttribute(L"w",val);

    //if linked then save dynamic attribution
    pDynamic=pDoc->createElement("dynamic");
    pGO->appendChild(pDynamic);
    if(m_DnyAttr.m_Linked){
        //linked
        val.vt=VT_BOOL;
        val.boolVal=-1;
        pDynamic->setAttribute(L"linked",val);
        //save dynamic attribution
        m_DnyAttr.Save(pDoc,pDynamic);
    }else{
        //unlinked
        val.vt=VT_BOOL;
        val.boolVal=FALSE;
        pDynamic->setAttribute(L"linked",val);
    }
}

void CRoundRect::MoveInCell(POINT potTracker)
{
	m_lx1+=potTracker.x;
	m_ly1+=potTracker.y;
	m_lx2+=potTracker.x;
	m_ly2+=potTracker.y;
}

void CRoundRect::MoveElement(POINT potOffset)
{
	m_lx1=m_lox1+potOffset.x;
	m_lx2=m_lox2+potOffset.x;
	m_ly1=m_loy1+potOffset.y;
	m_ly2=m_loy2+potOffset.y;
}

void CRoundRect::UpdateView()
{
	m_DnyAttr.m_na=false;

	float fp;
	long lp;

	//blink
	if(m_DnyAttr.m_Blink.m_Used){
		if(m_DnyAttr.m_Blink.m_TagValue.Flags & TF_Valid){
			bool bBlink=false;
			if(m_DnyAttr.m_Blink.m_VisibleAtOn && m_DnyAttr.m_Blink.m_TagValue.Value.iValue){
				bBlink=true;
			}else if(!m_DnyAttr.m_Blink.m_VisibleAtOn && !m_DnyAttr.m_Blink.m_TagValue.Value.iValue){
				bBlink=true;
			}else{
				bBlink=false;
			}
			if(m_DnyAttr.m_Blink.m_BlinkSpeed==0){
				if((glTimer%4)>1) bBlink=false;
			}else if(m_DnyAttr.m_Blink.m_BlinkSpeed==1){
				if((glTimer%8)>3) bBlink=false;
			}else{
				if((glTimer%16)>7) bBlink=false;
			}
			if(bBlink){
				m_pen.lopnColor=m_DnyAttr.m_Blink.m_ColorPen;
				m_brush.lbColor=m_DnyAttr.m_Blink.m_ColorBrush;
			}else{
				m_pen=m_open;
				m_brush=m_obrush;
			}
		}else{
			m_DnyAttr.m_na=true;
		}
	}

	//color brush a
	if(m_DnyAttr.m_ColorBrushA.m_Used){
		if(m_DnyAttr.m_ColorBrushA.m_TagValue.Flags & TF_Valid){
			__r4 r4 = getFloat(m_DnyAttr.m_ColorBrushA.m_TagValue);
			if(r4>m_DnyAttr.m_ColorBrushA.m_Split1){
				m_brush.lbColor=m_DnyAttr.m_ColorBrushA.m_Color0;
			}else if(r4>m_DnyAttr.m_ColorBrushA.m_Split2){
				m_brush.lbColor=m_DnyAttr.m_ColorBrushA.m_Color1;
			}else if(r4>m_DnyAttr.m_ColorBrushA.m_Split3){
				m_brush.lbColor=m_DnyAttr.m_ColorBrushA.m_Color2;
			}else if(r4>m_DnyAttr.m_ColorBrushA.m_Split4){
				m_brush.lbColor=m_DnyAttr.m_ColorBrushA.m_Color3;
			}else{
				m_brush.lbColor=m_DnyAttr.m_ColorBrushA.m_Color4;
			}
		}else{
			m_DnyAttr.m_na=true;
		}
	}
	//color brush aa
	if(m_DnyAttr.m_ColorBrushAA.m_Used){
		if(m_DnyAttr.m_ColorBrushAA.m_TagValue.Flags & TF_Valid){
			if(m_DnyAttr.m_ColorBrushAA.m_TagValue.Flags & TF_HiHi){
				m_brush.lbColor=m_DnyAttr.m_ColorBrushAA.m_ColorH2;
			}else if(m_DnyAttr.m_ColorBrushAA.m_TagValue.Flags & TF_Hi){
				m_brush.lbColor=m_DnyAttr.m_ColorBrushAA.m_ColorH1;
			}else if(m_DnyAttr.m_ColorBrushAA.m_TagValue.Flags & TF_LoLo){
				m_brush.lbColor=m_DnyAttr.m_ColorBrushAA.m_ColorL2;
			}else if(m_DnyAttr.m_ColorBrushAA.m_TagValue.Flags & TF_Lo){
				m_brush.lbColor=m_DnyAttr.m_ColorBrushAA.m_ColorL1;
			}else if(m_DnyAttr.m_ColorBrushAA.m_TagValue.Flags & TF_Rate){
				m_brush.lbColor=m_DnyAttr.m_ColorBrushAA.m_ColorR;
			}else{
				m_brush.lbColor=m_DnyAttr.m_ColorBrushAA.m_ColorN;
			}
		}else{
			m_DnyAttr.m_na=true;
		}
	}
	//color brush d
	if(m_DnyAttr.m_ColorBrushD.m_Used){
		if(m_DnyAttr.m_ColorBrushD.m_TagValue.Flags & TF_Valid){
			if(m_DnyAttr.m_ColorBrushD.m_TagValue.Value.iValue){
				m_brush.lbColor=m_DnyAttr.m_ColorBrushD.m_ColorOn;
			}else{
				m_brush.lbColor=m_DnyAttr.m_ColorBrushD.m_ColorOff;
			}
		}else{
			m_DnyAttr.m_na=true;
		}
	}
	//color brush da
	if(m_DnyAttr.m_ColorBrushDA.m_Used){
		if(m_DnyAttr.m_ColorBrushDA.m_TagValue.Flags & TF_Valid){
			if(m_DnyAttr.m_ColorBrushDA.m_TagValue.Flags & TF_Off2On){
				m_brush.lbColor=m_DnyAttr.m_ColorBrushDA.m_ColorAlarm;
			}else if(m_DnyAttr.m_ColorBrushDA.m_TagValue.Flags & TF_On2Off){
				m_brush.lbColor=m_DnyAttr.m_ColorBrushDA.m_ColorAlarm;
			}else{
				m_brush.lbColor=m_DnyAttr.m_ColorBrushDA.m_ColorNomal;
			}
		}else{
			m_DnyAttr.m_na=true;
		}
	}
	//color pen a
	if(m_DnyAttr.m_ColorPenA.m_Used){
		if(m_DnyAttr.m_ColorPenA.m_TagValue.Flags & TF_Valid){
			__r4 r4 = getFloat(m_DnyAttr.m_ColorPenA.m_TagValue);
			if(r4>m_DnyAttr.m_ColorPenA.m_Split1){
				m_pen.lopnColor=m_DnyAttr.m_ColorPenA.m_Color0;
			}else if(r4>m_DnyAttr.m_ColorPenA.m_Split2){
				m_pen.lopnColor=m_DnyAttr.m_ColorPenA.m_Color1;
			}else if(r4>m_DnyAttr.m_ColorPenA.m_Split3){
				m_pen.lopnColor=m_DnyAttr.m_ColorPenA.m_Color2;
			}else if(r4>m_DnyAttr.m_ColorPenA.m_Split4){
				m_pen.lopnColor=m_DnyAttr.m_ColorPenA.m_Color3;
			}else{
				m_pen.lopnColor=m_DnyAttr.m_ColorPenA.m_Color4;
			}
		}else{
			m_DnyAttr.m_na=true;
		}
	}
	//color pen aa
	if(m_DnyAttr.m_ColorPenAA.m_Used){
		if(m_DnyAttr.m_ColorPenAA.m_TagValue.Flags & TF_Valid){
			if(m_DnyAttr.m_ColorPenAA.m_TagValue.Flags & TF_HiHi){
				m_pen.lopnColor=m_DnyAttr.m_ColorPenAA.m_ColorH2;
			}else if(m_DnyAttr.m_ColorPenAA.m_TagValue.Flags & TF_Hi){
				m_pen.lopnColor=m_DnyAttr.m_ColorPenAA.m_ColorH1;
			}else if(m_DnyAttr.m_ColorPenAA.m_TagValue.Flags & TF_LoLo){
				m_pen.lopnColor=m_DnyAttr.m_ColorPenAA.m_ColorL2;
			}else if(m_DnyAttr.m_ColorPenAA.m_TagValue.Flags & TF_Lo){
				m_pen.lopnColor=m_DnyAttr.m_ColorPenAA.m_ColorL1;
			}else if(m_DnyAttr.m_ColorPenAA.m_TagValue.Flags & TF_Rate){
				m_pen.lopnColor=m_DnyAttr.m_ColorPenAA.m_ColorR;
			}else{
				m_pen.lopnColor=m_DnyAttr.m_ColorPenAA.m_ColorN;
			}
		}else{
			m_DnyAttr.m_na=true;
		}
	}
	//color pen d
	if(m_DnyAttr.m_ColorPenD.m_Used){
		if(m_DnyAttr.m_ColorPenD.m_TagValue.Flags & TF_Valid){
			if(m_DnyAttr.m_ColorBrushD.m_TagValue.Value.iValue){
				m_pen.lopnColor=m_DnyAttr.m_ColorBrushD.m_ColorOn;
			}else{
				m_pen.lopnColor=m_DnyAttr.m_ColorBrushD.m_ColorOff;
			}
		}else{
			m_DnyAttr.m_na=true;
		}
	}
	//color pen da
	if(m_DnyAttr.m_ColorPenDA.m_Used){
		if(m_DnyAttr.m_ColorPenDA.m_TagValue.Flags & TF_Valid){
			if(m_DnyAttr.m_ColorPenDA.m_TagValue.Flags & TF_Off2On){
				m_pen.lopnColor=m_DnyAttr.m_ColorPenDA.m_ColorAlarm;
			}else if(m_DnyAttr.m_ColorPenDA.m_TagValue.Flags & TF_On2Off){
				m_pen.lopnColor=m_DnyAttr.m_ColorPenDA.m_ColorAlarm;
			}else{
				m_pen.lopnColor=m_DnyAttr.m_ColorPenDA.m_ColorNomal;
			}
		}else{
			m_DnyAttr.m_na=true;
		}
	}
	//color text a
	//color text aa
	//color text d
	//color text da
	//direction

	//fill h
	if(m_DnyAttr.m_FillH.m_Used){
        if(!(m_DnyAttr.m_FillH.m_TagValue.Flags & TF_Valid)){
			m_DnyAttr.m_na=true;
        }
    }
	//fill v
	if(m_DnyAttr.m_FillV.m_Used){
        if(!(m_DnyAttr.m_FillV.m_TagValue.Flags & TF_Valid)){
			m_DnyAttr.m_na=true;
        }
    }

    long ltx1=m_lox1;
	long ltx2=m_lox2;
	long lty1=m_loy1;
	long lty2=m_loy2;
	//pos h
	if(m_DnyAttr.m_PosH.m_Used){
		if(m_DnyAttr.m_PosH.m_TagValue.Flags & TF_Valid){			
			lp=(getFloat(m_DnyAttr.m_PosH.m_TagValue)-m_DnyAttr.m_PosH.m_LTO)*\
				(m_DnyAttr.m_PosH.m_RBT-m_DnyAttr.m_PosH.m_LTT)/\
				(m_DnyAttr.m_PosH.m_RBO-m_DnyAttr.m_PosH.m_LTO);
			if(lp<m_DnyAttr.m_PosH.m_LTT) lp=m_DnyAttr.m_PosH.m_LTT;
			if(lp>m_DnyAttr.m_PosH.m_RBT) lp=m_DnyAttr.m_PosH.m_RBT;
			m_lx1=m_lox1+lp;
			m_lx2=m_lox2+lp;
			ltx1=m_lx1;
			ltx2=m_lx2;
		}else{
			m_DnyAttr.m_na=true;
		}
	}
	//pos v
	if(m_DnyAttr.m_PosV.m_Used){
		if(m_DnyAttr.m_PosV.m_TagValue.Flags & TF_Valid){			
			lp=(getFloat(m_DnyAttr.m_PosV.m_TagValue)-m_DnyAttr.m_PosV.m_LTO)*\
				(m_DnyAttr.m_PosV.m_RBT-m_DnyAttr.m_PosV.m_LTT)/\
				(m_DnyAttr.m_PosV.m_RBO-m_DnyAttr.m_PosV.m_LTO);
			if(lp<m_DnyAttr.m_PosV.m_LTT) lp=m_DnyAttr.m_PosV.m_LTT;
			if(lp>m_DnyAttr.m_PosV.m_RBT) lp=m_DnyAttr.m_PosV.m_RBT;
			m_ly1=m_loy1+lp;
			m_ly2=m_loy2+lp;
			lty1=m_ly1;
			lty2=m_ly2;
		}else{
			m_DnyAttr.m_na=true;
		}
	}
	//size h
	if(m_DnyAttr.m_SizeH.m_Used){
		if(m_DnyAttr.m_SizeH.m_TagValue.Flags & TF_Valid){
			fp=(getFloat(m_DnyAttr.m_SizeH.m_TagValue)-m_DnyAttr.m_SizeH.m_LTO)*\
				(m_DnyAttr.m_SizeH.m_RBT-m_DnyAttr.m_SizeH.m_LTT)/\
				(m_DnyAttr.m_SizeH.m_RBO-m_DnyAttr.m_SizeH.m_LTO)/100;
            fp += m_DnyAttr.m_SizeH.m_LTT/100;
			if(fp<0.) fp=0.;
			if(fp>1.) fp=1.;
			if(m_DnyAttr.m_SizeH.m_Ref==0){
				m_lx2=m_lx1+(m_lox2-m_lox1)*fp;
			}else if(m_DnyAttr.m_SizeH.m_Ref==1){
				m_lx1=(ltx2+ltx1)/2-(m_lox2-m_lox1)*fp/2.;
				m_lx2=(ltx2+ltx1)/2+(m_lox2-m_lox1)*fp/2.;
			}else if(m_DnyAttr.m_SizeH.m_Ref==2){
				m_lx1=m_lx2-(m_lox2-m_lox1)*fp;
			}
		}else{
			m_DnyAttr.m_na=true;
		}
	}
	//size v
	if(m_DnyAttr.m_SizeV.m_Used){
		if(m_DnyAttr.m_SizeV.m_TagValue.Flags & TF_Valid){
			fp=(getFloat(m_DnyAttr.m_SizeV.m_TagValue)-m_DnyAttr.m_SizeV.m_LTO)*\
				(m_DnyAttr.m_SizeV.m_RBT-m_DnyAttr.m_SizeV.m_LTT)/\
				(m_DnyAttr.m_SizeV.m_RBO-m_DnyAttr.m_SizeV.m_LTO)/100;
            fp += m_DnyAttr.m_SizeV.m_LTT/100;
			if(fp<0.) fp=0.;
			if(fp>100.) fp=100.;
			if(m_DnyAttr.m_SizeV.m_Ref==0){
				m_ly2=m_ly1+(m_loy2-m_loy1)*fp;
			}else if(m_DnyAttr.m_SizeV.m_Ref==1){
				m_ly1=(lty1+lty2)/2-(m_loy2-m_loy1)*fp/2.;
				m_ly2=(lty1+lty2)/2+(m_loy2-m_loy1)*fp/2.;
			}else if(m_DnyAttr.m_SizeV.m_Ref==2){
				m_ly1=m_ly2-(m_loy2-m_loy1)*fp;
			}
		}else{
			m_DnyAttr.m_na=true;
		}
	}
	//value a
	//value d
	//visible
	if(m_DnyAttr.m_Visible.m_Used){
		if(m_DnyAttr.m_Visible.m_TagValue.Flags & TF_Valid){
			if(m_DnyAttr.m_Visible.m_Visble && m_DnyAttr.m_Visible.m_TagValue.Value.iValue){
				m_DnyAttr.m_vi=true;
			}else if(!m_DnyAttr.m_Visible.m_Visble && !m_DnyAttr.m_Visible.m_TagValue.Value.iValue){
				m_DnyAttr.m_vi=true;
			}else{
				m_DnyAttr.m_vi=false;
			}
		}else{
			m_DnyAttr.m_na=true;
		}
	}

	if(m_DnyAttr.m_rank>glRank){
		m_DnyAttr.m_en=false;
	}else{
		m_DnyAttr.m_en=true;
	}
	//enable
	if(m_DnyAttr.m_Enable.m_Used){
		if(m_DnyAttr.m_Enable.m_TagValue.Flags & TF_Valid){
			if(m_DnyAttr.m_Enable.m_TagValue.Value.iValue && m_DnyAttr.m_Enable.m_Enable){
			}else if(!m_DnyAttr.m_Enable.m_TagValue.Value.iValue && !m_DnyAttr.m_Enable.m_Enable){
			}else{
				m_DnyAttr.m_en=false;
			}
		}else{
			m_DnyAttr.m_na=true;
			m_DnyAttr.m_en=false;
		}
	}
	//button a
	if(m_DnyAttr.m_ButtonA.m_Used){
		if(!(m_DnyAttr.m_ButtonA.m_TagValue.Flags & TF_Valid)){
			m_DnyAttr.m_en=false;
		}
	}
	//button d
	if(m_DnyAttr.m_ButtonD.m_Used){
		if(!(m_DnyAttr.m_ButtonD.m_TagValue.Flags & TF_Valid)){
			m_DnyAttr.m_en=false;
		}
	}
	//input a
	if(m_DnyAttr.m_InputA.m_Used){
		if(!(m_DnyAttr.m_InputA.m_TagValue.Flags & TF_Valid)){
			m_DnyAttr.m_en=false;
		}
	}
	//input d
	if(m_DnyAttr.m_InputD.m_Used){
		if(!(m_DnyAttr.m_InputD.m_TagValue.Flags & TF_Valid)){
			m_DnyAttr.m_en=false;
		}
	}
	//slide h
	if(m_DnyAttr.m_SlideH.m_Used){
		if(!(m_DnyAttr.m_SlideH.m_TagValue.Flags & TF_Valid)){
			m_DnyAttr.m_en=false;
		}
	}
	//slide v
	if(m_DnyAttr.m_SlideV.m_Used){
		if(!(m_DnyAttr.m_SlideV.m_TagValue.Flags & TF_Valid)){
			m_DnyAttr.m_en=false;
		}
	}
}