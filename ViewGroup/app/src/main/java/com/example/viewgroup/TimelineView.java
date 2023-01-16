package com.example.viewgroup;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;

import androidx.annotation.Nullable;

public class TimelineView extends View {
    class TimelineContext {
        public double mZoom = 6.0e-4;
        public double mDistance = 0.0f;
        public int mControlBarWidth = 100;
        long mLeftTime = 0;
        long mRightTime = 0;
    }

    enum Mode {
        Move,
        Zoom,
        NONE
    }
    private Mode mode = Mode.NONE;
    private final int BASE_LINE = 0;
    private final float ZOOM_RATIO = 1.22f;
    private final float MAX_ZOOM = 500.0f;
    private final float MIN_ZOOM = 1e-10f;
    private final int TICK_HEIGHT = 25;
    private final float TEXT_SPACING = 1.5f;

    private float mLastDistance = 0.0f;
    private float mPressedX = 0.0f;
    private Paint mPaint = new Paint();
    private TimelineContext mContext = new TimelineContext();
    private long mMinStep = 1;
    int mUnitTickPixel = 1;
    public TimelineView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        Layout();
    }

    private void Layout(){
        if (mContext.mZoom == 0.0f) {
            return;
        }
        long leftTime = (long)(-mContext.mDistance / mContext.mZoom);
        long rightTime = (long)((double)(getMeasuredWidth() - mContext.mControlBarWidth - mContext.mDistance) /
                mContext.mZoom);
        mContext.mLeftTime = leftTime;
        mContext.mRightTime = rightTime;
        long tickOnScreen = rightTime - leftTime;
        long perPixel2Ns = tickOnScreen / getMeasuredWidth();
        mMinStep = 1;
        while(perPixel2Ns > mMinStep || mMinStep * mContext.mZoom < 20) {
            mMinStep *= 10;
        }
        mUnitTickPixel = (int)(mContext.mZoom * mMinStep);
        invalidate();
    }

    String TransTime2String(long nsVal)
    {
        //if (nsVal < 0 || natureShow == 0) {
        //    return QString::number(nsVal);
        //}
        int zero = 0;
        long tmpValue = nsVal;
        while (tmpValue > 0) {
            if (tmpValue % 10 == 0) {
                zero++;
                tmpValue /= 10;
            } else {
                break;
            }
        }
        String resultVal;
        if (zero >= 9) {
//            resultVal = String::number(nsVal / 1000000000) + "s";
            resultVal = (nsVal / 1000000000) + "s";
        } else if (zero >= 6) {
            resultVal = (nsVal / 1000000) + "ms";
        } else if (zero >= 3) {
            resultVal = (nsVal / 1000) + "us";
        } else {
            resultVal = (nsVal) + "ns";
        }
        return resultVal;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        mPaint.setTextSize(30);
//        mPaint.setColor(Color.argb(155, 123, 123,123));
//        canvas.drawRect(new Rect(0, 0, getMeasuredWidth(),getMeasuredHeight()), mPaint);
        long startTick = mContext.mLeftTime - (mContext.mLeftTime % mMinStep) - mMinStep;
        String maxShow = TransTime2String(mContext.mRightTime);
        float maxTextWidth = mPaint.measureText(maxShow);
        for (long i = startTick; i < mContext.mRightTime + mMinStep; i += mMinStep) {
            double onScreenX = (double)(i) * mContext.mZoom
                    + mContext.mDistance + mContext.mControlBarWidth;
            if (onScreenX < mContext.mControlBarWidth) {
                continue;
            }
            canvas.drawLine((int)onScreenX, BASE_LINE, (int)onScreenX, BASE_LINE + TICK_HEIGHT, mPaint);

            if (i % (mMinStep * 100) == 0) {
                String show = TransTime2String(i);
                float textWidth = mPaint.measureText(show);
                Paint.FontMetrics fm = mPaint.getFontMetrics();
                float textHeight = fm.descent - fm.ascent;
                if (mUnitTickPixel * 100 > textWidth * TEXT_SPACING) {
                    canvas.drawText(show, (float) (onScreenX - textWidth / 2.0f), BASE_LINE + TICK_HEIGHT + textHeight, mPaint);
                }
                continue;
            }

            if (i % (mMinStep * 10) == 0) {
                String show = TransTime2String(i);
                float textWidth = mPaint.measureText(show);
                Paint.FontMetrics fm = mPaint.getFontMetrics();
                float textHeight = fm.descent - fm.ascent;
                if (mUnitTickPixel * 10 > maxTextWidth * TEXT_SPACING) {
//                    p.drawText(onScreenX - textWidth / 2, BASE_LINE + TICK_HEIGHT + fm.height(), show);
                    canvas.drawText(show, (float) (onScreenX - textWidth / 2.0f), BASE_LINE + TICK_HEIGHT + textHeight, mPaint);
                } else {
                    show = TransTime2String(i % (mMinStep * 100));
                    textWidth = mPaint.measureText(show);
                    textHeight = fm.descent - fm.ascent;
                    if (mUnitTickPixel * 10 > textWidth * TEXT_SPACING) {
//                        p.drawText(onScreenX - textWidth / 2, BASE_LINE + TICK_HEIGHT + fm.height(), show);
                        canvas.drawText(show, (float) (onScreenX - textWidth / 2.0f), BASE_LINE + TICK_HEIGHT + textHeight, mPaint);
                    }
                }
                continue;
            }

            if (i % (mMinStep * 5) == 0) {
                String show = TransTime2String(i);
                float textWidth = mPaint.measureText(show);
                Paint.FontMetrics fm = mPaint.getFontMetrics();
                float textHeight = fm.descent - fm.ascent;
                if (mUnitTickPixel * 5 > maxTextWidth * TEXT_SPACING) {
                    canvas.drawText(show, (float) (onScreenX - textWidth / 2.0f), BASE_LINE + TICK_HEIGHT + textHeight, mPaint);
                }
                continue;
            }

            String show = TransTime2String(i);
            float textWidth = mPaint.measureText(show);
            Paint.FontMetrics fm = mPaint.getFontMetrics();
            float textHeight = fm.descent - fm.ascent;
            if (mUnitTickPixel > maxTextWidth * TEXT_SPACING) {
                canvas.drawText(show, (float) (onScreenX - textWidth / 2.0f), BASE_LINE + TICK_HEIGHT + textHeight, mPaint);
            }
        }
    }

    private void onPointDown(MotionEvent e)
    {
        if (e.getPointerCount() == 1) {
            mode = Mode.Move;
            mPressedX = e.getX();
        }
        if (e.getPointerCount() == 2) {
            mode = Mode.Zoom;
            Log.d("suncaiyu",e.getX(0) + "");
            Log.d("suncaiyu",e.getX(1) + "");
            mLastDistance = Math.abs(e.getX(0) - e.getX(1));
        }
    }

    private void onTouchMove(MotionEvent e)
    {
        if (mode == Mode.Zoom) {
            if (e.getPointerCount() != 2) {
                return;
            }
            float nowDistahce = Math.abs(e.getX(0) - e.getX(1));
            float mousePos = (e.getX(0) + e.getX(1)) / 2.0f;
            double mouseOnTimePoint = (mousePos - mContext.mDistance - mContext.mControlBarWidth)
                    / mContext.mZoom;
            if (nowDistahce < mLastDistance) {
                //zoom in
                mContext.mZoom = mContext.mZoom / ZOOM_RATIO < MIN_ZOOM ? MIN_ZOOM : mContext.mZoom / ZOOM_RATIO;
                mContext.mDistance = -(mouseOnTimePoint * mContext.mZoom - mousePos + mContext.mControlBarWidth);
            } else if (nowDistahce > mLastDistance){
                // zoom out

                mContext.mZoom = mContext.mZoom * ZOOM_RATIO > MAX_ZOOM ? MAX_ZOOM : mContext.mZoom * ZOOM_RATIO;
                mContext.mDistance = -(mouseOnTimePoint * mContext.mZoom - mousePos + mContext.mControlBarWidth);
            }
            mLastDistance = nowDistahce;
        } else if (mode == Mode.Move) {
            float distance = e.getX() - mPressedX;
            mPressedX = e.getX();
            // 暂定可向左拖动1000像素
            if (mContext.mDistance + distance < 1000) {
                mContext.mDistance = mContext.mDistance + distance;
            } else {
                mContext.mDistance = 1000;
            }
        }
        Layout();
    }

    @Override
    public boolean onTouchEvent(MotionEvent e) {
        switch (e.getAction() & MotionEvent.ACTION_MASK) {
            case MotionEvent.ACTION_POINTER_DOWN:
                onPointDown(e);
                break;
            case MotionEvent.ACTION_DOWN:
                onPointDown(e);
                break;
            case MotionEvent.ACTION_MOVE:
                onTouchMove(e);
                break;
            case MotionEvent.ACTION_UP:
                mode = Mode.NONE;
//                mLastDistance = 0.0f;
                break;
            case MotionEvent.ACTION_POINTER_UP:
                mode = Mode.NONE;
                mLastDistance = 0.0f;
                break;
            default:
                break;
        }
        return true;
    }
}
