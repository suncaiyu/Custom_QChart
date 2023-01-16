package com.example.viewgroup;

import android.content.Context;
import android.content.res.TypedArray;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;

public class FlowLayout extends ViewGroup {

    public class Location {
        public Location(int left, int top, int right, int bottom) {
            this.left = left;
            this.top = top;
            this.right = right;
            this.bottom = bottom;
        }
        public int left;
        public int top;
        public int right;
        public int bottom;
    }

    private int childHorizontalSpace;
    private int childVerticalSpace;
    public FlowLayout(Context context) {
        super(context);
//        Log.d("suncaiyu", "FlowLayout(Context context)");
    }

    public FlowLayout(Context context, AttributeSet attrs) {
        super(context, attrs);
//        Log.d("suncaiyu", "FlowLayout(Context context, AttributeSet attrs)");
        TypedArray arry = context.obtainStyledAttributes(attrs, R.styleable.FlowLayout);
        if (arry != null) {
            childHorizontalSpace = arry.getDimensionPixelSize(R.styleable.FlowLayout_horizontalSpace, 12);
            childVerticalSpace = arry.getDimensionPixelSize(R.styleable.FlowLayout_verticalSpace, 12);
            arry.recycle();
        }
        this.setPadding(60, 60, 0, 0);
    }

    public FlowLayout(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
//        Log.d("suncaiyu", "FlowLayout(Context context, AttributeSet attrs, int defStyleAttr)");
    }

    public FlowLayout(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
//        Log.d("suncaiyu", "FlowLayout(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes)");
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        Log.e(getClass().getName(),"onMeasure");

        int sizeWidth = MeasureSpec.getSize(widthMeasureSpec);
        int sizeHeight = MeasureSpec.getSize(heightMeasureSpec);
        int modeWidth = MeasureSpec.getMode(widthMeasureSpec);
        int modeHeight = MeasureSpec.getMode(heightMeasureSpec);

        int width = 0;
        int height = 0;
        int lineWidth = 0;
        int lineHeight = 0;

        int count = getChildCount();
        int left = getPaddingLeft();
        int top = getPaddingTop();

        for (int i = 0; i < count; i++) {
            View child = getChildAt(i);
            if (child.getVisibility() == GONE) {
                continue;
            }
            measureChild(child, widthMeasureSpec, heightMeasureSpec);
            ViewGroup.LayoutParams lp = child.getLayoutParams();
            int childWidth = child.getMeasuredWidth() + childHorizontalSpace;
            int childHeight = child.getMeasuredHeight() + childVerticalSpace;

            if (lineWidth + childWidth > sizeWidth - left - getPaddingRight()) {
                Log.d("suncaiyu", lineWidth + childWidth + "");
                width = Math.max(lineWidth, childWidth);// 取最大的
                lineWidth = childWidth; // 重新开启新行，开始记录
                // 叠加当前高度，
                height += lineHeight;
                // 开启记录下一行的高度
                lineHeight = 0;
                child.setTag(new Location(left, top + height, childWidth + left - childHorizontalSpace, height + child.getMeasuredHeight() + top));
            } else {
                // 否则累加值lineWidth,lineHeight取最大高度
                child.setTag(new Location(lineWidth + left, top + height, lineWidth + childWidth - childHorizontalSpace + left, height + child.getMeasuredHeight() + top));
                lineWidth += childWidth;
                lineHeight = Math.max(lineHeight, childHeight);
            }
            setMeasuredDimension((modeWidth == MeasureSpec.EXACTLY) ? sizeWidth : width, (modeHeight == MeasureSpec.EXACTLY) ? sizeHeight : height);
        }
    }

    @Override
    protected void onLayout(boolean changed, int l, int t, int r, int b) {
        Log.e(getClass().getName(),"onLayout");

        int count = getChildCount();
        for (int i = 0; i < count; i++) {
            //获取子View
            View child = getChildAt(i);
            //判断是否显示
            if (child.getVisibility() == GONE)
                continue;
            //获取子View的坐标
            Location location = (Location) child.getTag();
            //设置子View位置
            child.layout(location.left, location.top, location.right, location.bottom);
        }
    }
}
