package com.example.caculate;

import java.util.Stack;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity implements OnClickListener {

	static{
		System.loadLibrary("JniTest");
	}

	private native int Add(double num1,double num2);
	private native int Sub(double num1,double num2);
	private native int Mul(double num1,double num2);
	private native int Div(double num1,double num2);


	Stack<String> optr = new Stack<String>();
	Stack<Double> opnd = new Stack<Double>();

	TextView display = null;
	TextView answer=null;
	int index_begin=-1;
	boolean press_answer_flag=false;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		display = (TextView) findViewById(R.id.output_window);
		answer = (TextView) findViewById(R.id.answer_window);
		
		Button num0 = (Button) findViewById(R.id.zero_button);
		Button num1 = (Button) findViewById(R.id.one_button);
		Button num2 = (Button) findViewById(R.id.two_button);
		Button num3 = (Button) findViewById(R.id.three_button);
		Button num4 = (Button) findViewById(R.id.four_button);
		Button num5 = (Button) findViewById(R.id.five_button);
		Button num6 = (Button) findViewById(R.id.six_button);
		Button num7 = (Button) findViewById(R.id.seven_button);
		Button num8 = (Button) findViewById(R.id.eight_button);
		Button num9 = (Button) findViewById(R.id.nine_button);
		Button point = (Button) findViewById(R.id.point_button);

		Button btnDel = (Button) findViewById(R.id.backspace_button);
		Button btnEql = (Button) findViewById(R.id.equal_button);

		Button btnPlu = (Button) findViewById(R.id.plus_button);
		Button btnMin = (Button) findViewById(R.id.minus_button);
		Button btnMul = (Button) findViewById(R.id.product_button);
		Button btnDiv = (Button) findViewById(R.id.divisor_button);
		Button btnLeft = (Button) findViewById(R.id.left_bracket_button);
		Button btnRight = (Button) findViewById(R.id.right_bracket_button);

		num0.setOnClickListener(this);
		num1.setOnClickListener(this);
		num2.setOnClickListener(this);
		num3.setOnClickListener(this);
		num4.setOnClickListener(this);
		num5.setOnClickListener(this);
		num6.setOnClickListener(this);
		num7.setOnClickListener(this);
		num8.setOnClickListener(this);
		num9.setOnClickListener(this);
		point.setOnClickListener(this);
		
		btnDel.setOnClickListener(this);
		btnEql.setOnClickListener(this);
		
		btnPlu.setOnClickListener(this);
		btnMin.setOnClickListener(this);
		btnMul.setOnClickListener(this);
		btnDiv.setOnClickListener(this);
		btnLeft.setOnClickListener(this);
		btnRight.setOnClickListener(this);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	@Override
	public void onClick(View arg0) {
		// TODO Auto-generated method stub
		Button btn = (Button) arg0;
		String show = display.getText().toString();
		String temp=null;
		
		if (btn.getText().equals("0") || btn.getText().equals("1")
				|| btn.getText().equals("2") || btn.getText().equals("3")
				|| btn.getText().equals("4") || btn.getText().equals("5")
				|| btn.getText().equals("6") || btn.getText().equals("7")
				|| btn.getText().equals("8") || btn.getText().equals("9")|| btn.getText().equals("."))
		{
			if(press_answer_flag)
			{
				display.setText("");
				answer.setText("");
				optr.clear();
				opnd.clear();
				index_begin=-1;
				press_answer_flag=false;
				show="";
			}
			show += btn.getText();
			display.setText(show);
			if(index_begin==-1)
			{
				index_begin=show.lastIndexOf(btn.getText().toString());
				//Toast.makeText(this, "index_begin is changed from -1 to "+index_begin, Toast.LENGTH_LONG).show();
			}				
		}
		else if(btn.getText().equals("+")||btn.getText().equals("-")||btn.getText().equals("*")
			||btn.getText().equals("/")||btn.getText().equals("(")||btn.getText().equals(")"))
		{
			if(press_answer_flag)
			{
				display.setText(answer.getText());
				index_begin=-1;
				press_answer_flag=false;
				show=answer.getText().toString();
				answer.setText("");
			}
			if(index_begin!=-1)
			{
				temp=show.substring(index_begin);
				opnd.push(Double.valueOf(temp).doubleValue());
				index_begin=-1;
				//Toast.makeText(this, "temp is"+temp+". push into opnd,the number is "+Double.valueOf(temp).doubleValue(), Toast.LENGTH_LONG).show();
			}
			show+=btn.getText();
			display.setText(show);
			
			temp=btn.getText().toString();
			while(!optr.empty()&&!lesser(optr.peek(),temp))
			{
				switch (precede(optr.peek(), temp)) {
				case '<':
					optr.push(temp);
					break;
				case '=':
					optr.pop();
					return;
				case '>':
					double a,b,c;
					b = opnd.pop();
					a = opnd.pop();
					String theta = optr.pop();
					c = operate(a, theta, b);
					opnd.push(c);
					break;
				case '#':
					show = "ERROR";
					display.setText(show);
					optr.clear();
					opnd.clear();
					break;
				}
			}
			if(optr.empty()||lesser(optr.peek(),temp))
				optr.push(temp);
		}
		else if(btn.getText().equals("="))
		{
			press_answer_flag=true;
			if(index_begin!=-1)
			{
				temp=show.substring(index_begin);
				opnd.push(Double.valueOf(temp).doubleValue());
				index_begin=-1;
				//Toast.makeText(this, "temp is"+temp+". push into opnd,the number is "+Double.valueOf(temp).doubleValue(), Toast.LENGTH_LONG).show();
			}		
			show+=btn.getText().toString();
			display.setText(show);
			
			while(!optr.isEmpty())
			{
				double a,b,c;
				b = opnd.pop();
				a = opnd.pop();
				String theta = optr.pop();
				c = operate(a, theta, b);
				opnd.push(c);
			}
			answer.setText(opnd.peek().toString());
		}
		else if(btn.getText().equals("C"))
		{
			display.setText("");
			answer.setText("");
			optr.clear();
			opnd.clear();
			index_begin=-1;
			press_answer_flag=false;
		}
	}



	private boolean lesser(String theta, String temp) {
		// TODO Auto-generated method stub
		switch(precede(theta,temp))
		{
		case '<':return true;
		case '=':return false;
		case '>':return false;
		}
		return false;
	}

	private double operate(double a, String theta, double b) {
		// TODO Auto-generated method stub
		if(theta.equals("+"))
			return Add(a,b);
		else if(theta.equals("-"))
			return Sub(a,b);
		else if(theta.equals("*"))
			return Mul(a,b);
		else if(theta.equals("/"))
			return Div(a,b);
		return 0;
	}

	private char precede(String theta, String temp) {
		// TODO Auto-generated method stub
		if (theta.equals("+")) {
			if (temp.equals("+"))
				return '>';
			else if (temp.equals("-"))
				return '>';
			else if (temp.equals("*"))
				return '<';
			else if (temp.equals("/"))
				return '<';
			else if (temp.equals("("))
				return '<';
			else if (temp.equals(")"))
				return '>';
		}
		else if(theta.equals("-")){
			if (temp.equals("+"))
				return '>';
			else if (temp.equals("-"))
				return '>';
			else if (temp.equals("*"))
				return '<';
			else if (temp.equals("/"))
				return '<';
			else if (temp.equals("("))
				return '<';
			else if (temp.equals(")"))
				return '>';
		}
		else if(theta.equals("*")){
			if (temp.equals("+"))
				return '>';
			else if (temp.equals("-"))
				return '>';
			else if (temp.equals("*"))
				return '>';
			else if (temp.equals("/"))
				return '>';
			else if (temp.equals("("))
				return '<';
			else if (temp.equals(")"))
				return '>';
		}
		else if(theta.equals("/")){
			if (temp.equals("+"))
				return '>';
			else if (temp.equals("-"))
				return '>';
			else if (temp.equals("*"))
				return '>';
			else if (temp.equals("/"))
				return '>';
			else if (temp.equals("("))
				return '<';
			else if (temp.equals(")"))
				return '>';
		}
		else if(theta.equals("(")){
			if (temp.equals("+"))
				return '<';
			else if (temp.equals("-"))
				return '<';
			else if (temp.equals("*"))
				return '<';
			else if (temp.equals("/"))
				return '<';
			else if (temp.equals("("))
				return '<';
			else if (temp.equals(")"))
				return '=';
		}
		else if(theta.equals(")")){
			if (temp.equals("+"))
				return '>';
			else if (temp.equals("-"))
				return '>';
			else if (temp.equals("*"))
				return '>';
			else if (temp.equals("/"))
				return '>';
			else if (temp.equals("("))
				return '#';
			else if (temp.equals(")"))
				return '>';
		}
		return '#';
	}
	
}
