#coding=utf-8
import MVGigE 
import sys

dev_num = MVGigE.GetNumOfDevices()

print("find %d cameras" % dev_num)

for i in range(dev_num):
	info = MVGigE.GetDevInfo(i)
	print(i, " : %d.%d.%d.%d" % (info['mIpAddr'][0], \
		info['mIpAddr'][1], info['mIpAddr'][2], info['mIpAddr'][3]))
		
	if (info['mIpAddr'][0] != info['m_IfIp'][0]) \
			or ((info['mIpAddr'][1] != info['m_IfIp'][1])) \
			or ((info['mIpAddr'][2] != info['m_IfIp'][2])):
		info['mIpAddr'][0] = info['m_IfIp'][0]
		info['mIpAddr'][1] = info['m_IfIp'][1]
		info['mIpAddr'][2] = info['m_IfIp'][2]
		info['mIpAddr'][3] = info['m_IfIp'][3] + 1
			
		mac = b"%c%c%c%c%c%c" % (int(info["mEthernetAddr"][0]), \
				int(info["mEthernetAddr"][1]), int(info["mEthernetAddr"][2]),    \
				int(info["mEthernetAddr"][3]), int(info["mEthernetAddr"][4]),    \
				int(info["mEthernetAddr"][5]))

		str_ip = b'%d.%d.%d.%d' % (info['mIpAddr'][0], \
			info['mIpAddr'][1], info['mIpAddr'][2], \
			info['mIpAddr'][3])

		gateway = b'%d.%d.%d.%d' % (info['mIpAddr'][0], \
			info['mIpAddr'][1], info['mIpAddr'][2], \
			1)
			
		print(i, " forceip: %d.%d.%d.%d" % (info['mIpAddr'][0], \
		info['mIpAddr'][1], info['mIpAddr'][2], info['mIpAddr'][3]))
		
		if(MVGigE.ForceIp(mac, \
			str_ip, \
			b'255.255.255.0', b'0.0.0.0') != 0):
			print(i, ' forceip error')

	#ip = b"192.168.11.60"
	#gateway = b"192.168.11.1"
	#subnet_mask = b"255.255.255.0"
	#mac = bytes([info["mEthernetAddr"][0], \
	#		info["mEthernetAddr"][1], info["mEthernetAddr"][2],    \
	#		info["mEthernetAddr"][3], info["mEthernetAddr"][4],    \
	#		info["mEthernetAddr"][5]])
	#mac = bytes("%c%c%c%c%c%c" % (int(info["mEthernetAddr"][0]), \
	##		int(info["mEthernetAddr"][1]), int(info["mEthernetAddr"][2]),    \
	#		int(info["mEthernetAddr"][3]), int(info["mEthernetAddr"][4]),    \
	#		int(info["mEthernetAddr"][5])))
	#print(mac)

print("==================================")

dev_num = MVGigE.GetNumOfCameras()

print("find %d cameras" % dev_num)

for i in range(dev_num):
	info = MVGigE.GetCameraInfo(i);
	print("[%d]IP : %d.%d.%d.%d" % (i, info["mIpAddr"][0],   \
		info["mIpAddr"][1], info["mIpAddr"][2],  \
		info["mIpAddr"][3]))




