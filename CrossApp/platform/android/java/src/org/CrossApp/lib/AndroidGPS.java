package org.CrossApp.lib;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.Intent;
import android.location.Criteria;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.provider.Settings;
import android.util.Log;
import android.widget.Toast;

public class AndroidGPS {
	private static Activity s_pContext;
	public static void Init( final Activity context )
	{
		s_pContext = context;
		openGPSSettings();
	}
	public static void openGPSSettings()
	{
        LocationManager alm = (LocationManager) s_pContext.getSystemService(Context.LOCATION_SERVICE);
        if (alm.isProviderEnabled(android.location.LocationManager.GPS_PROVIDER)) {
            //Toast.makeText(s_pContext, "GPSģ������", Toast.LENGTH_SHORT).show();
            return;
        }
        //Toast.makeText(s_pContext, "�뿪��GPS��", Toast.LENGTH_SHORT).show();
        Intent intent = new Intent(Settings.ACTION_LOCATION_SOURCE_SETTINGS);
        s_pContext.startActivityForResult(intent,10); //��Ϊ������ɺ󷵻ص���ȡ����
    }

	public static double[] getLocation()
    {
        // ��ȡλ�ù������
        LocationManager locationManager;
        String serviceName = Context.LOCATION_SERVICE;
        locationManager = (LocationManager) s_pContext.getSystemService(serviceName);
        // ���ҵ�������Ϣ
        Criteria criteria = new Criteria();
        criteria.setAccuracy(Criteria.ACCURACY_FINE); // �߾���
        criteria.setAltitudeRequired(false);
        criteria.setBearingRequired(false);
        criteria.setCostAllowed(true);
        criteria.setPowerRequirement(Criteria.POWER_LOW); // �͹���
       
        String provider = locationManager.getBestProvider(criteria, true); // ��ȡGPS��Ϣ
        Location location = locationManager.getLastKnownLocation(provider); // ͨ��GPS��ȡλ��
        // ���ü��������Զ����µ���Сʱ��Ϊ���N��(1��Ϊ1*1000������д��ҪΪ�˷���)����Сλ�Ʊ仯����N��
        if (location == null) {
        	locationManager.requestLocationUpdates(provider, 100 * 1000, 500,locationListener);
		}

        return new double[]{location.getLatitude() , location.getLongitude() };
    }
	
	private static final LocationListener locationListener = new LocationListener() { 
		@Override 
		public void onStatusChanged(String provider, int status, Bundle extras) { 
		} 
		@Override 
		public void onProviderEnabled(String provider) { 
		} 
		@Override 
		public void onProviderDisabled(String provider) { 
		} 
		//��λ�ñ仯ʱ���� 
		@Override 
		public void onLocationChanged(Location location) { 
		//ʹ���µ�location����TextView��ʾ 
			System.out.println("en here!!!!");
		//updateToNewLocation(location); 
		}
		}; 
	private static void updateToNewLocation(Location location) {

        if (location != null) {
            double  latitude = location.getLatitude();
            double longitude= location.getLongitude();
            //tv1.setText("ά�ȣ�" +  latitude+ "\n����" + longitude);
        } else {
            //tv1.setText("�޷���ȡ������Ϣ");
        }

    }
}
