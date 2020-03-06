/*
 * LoadRunner Java script. (Build: _build_number_)
 * 
 * Script Description: 
 *                     
 */

import lrapi.lr;
import java.io.*;
import java.util.Random;
import java.sql.*;
import java.text.SimpleDateFormat;  
import java.util.Date;  

public class Actions
{

	String dbUrl = "jdbc:oracle:thin:@//localhost/xe";
	String user = "system";
	String pass = "admin";
	Connection conn = null;
	Statement stat = null;
	
	public int init() throws Throwable {
		Class.forName("oracle.jdbc.OracleDriver").newInstance();
		conn = DriverManager.getConnection(dbUrl, user, pass);
		stat = conn.createStatement(ResultSet.TYPE_SCROLL_SENSITIVE, ResultSet.CONCUR_READ_ONLY);
		return 0;
	}//end of init


	public int action() throws Throwable {
		try {
			Random rand = new Random();
			int r = rand.nextInt(3) + 1;
			SimpleDateFormat df = new SimpleDateFormat("dd.MM.yyyy");
			Date dateobj = new Date();
			switch(r) {
				case 1: {
					lr.start_transaction("INSERT");
					String q = "insert into pflbtd(currentDate, randomString, threadNum, iterNum) " +
					                   "values ('" + df.format(dateobj) + 
					                   "', '" + lr.eval_string("randString") + 
					                   "', " + lr.eval_int("vid") +
					                   ", " + lr.eval_int("iter") + ")";
					lr.output_message(q);
					stat.executeUpdate(q);
					lr.end_transaction("INSERT", lr.PASS);
					break;
				}
				case 2: {
					lr.start_transaction("SELECT");
					ResultSet rs = stat.executeQuery("SELECT * FROM pflbtd where iterNum = " + lr.eval_int("iter") + " - 1");
					int size = 0;
					if (rs.last()) 
					{
						size = rs.getRow(); // get row id 
						rs.beforeFirst();
					}
					lr.output_message(size + "");
					if(size % 3 == 0 && size > 0) {
						try {
							File f = new File(lr.eval_string("C:\\Games\\log.txt"));
							BufferedWriter bw = new BufferedWriter(new FileWriter(f, true));
							while (rs.next()) {
								//currentDate, randomString, threadNum, iterNum
				                int id = rs.getInt("id");
				                String date = rs.getDate("currentDate") + "";
				                String s = rs.getString("randomString");
				                int thr = rs.getInt("threadNum");
				                int iter = rs.getInt("iterNum");
				                bw.write(id + "," + date + "," + s + "," + thr + "," + iter + "\n");
				            }
							bw.close();
						} catch(IOException e) {
							e.printStackTrace();
						}
					}
					lr.end_transaction("SELECT", lr.PASS);
					break;
				}
				case 3: {
					lr.start_transaction("UPDATE");
					int rr = 1;
					if(lr.eval_int("vid") > 0)
						rr = rand.nextInt(lr.eval_int("vid")) + 1;
					
					stat.executeUpdate("update pflbtd set currentDate = " +
					                   "'" + df.format(dateobj) + "'" + 
					                   ", randomString = '" + lr.eval_string("randString") + "'" +
					                   ", threadNum = " + lr.eval_int("vid") +
					                   ", iterNum = " + lr.eval_int("iter") +
									   "where threadNum = " + rr);
					lr.end_transaction("UPDATE", lr.PASS);
					break;
				}
				default: {
					break;
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return 0;
	}//end of action


	public int end() throws Throwable {
		stat.close();
		conn.close();
		return 0;
	}//end of end
}
