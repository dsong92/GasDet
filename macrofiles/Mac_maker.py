#!/usr/bin/env python
# -*- coding: utf-8 -*-
import csv
import numpy as np
from scipy.interpolate import interp2d
from scipy.interpolate import interp1d
from matplotlib import pyplot as plt
import pandas as pd
import os
import re
import subprocess

# 폴더 내 엑셀파일 이름 불러오기
path = "./"
file_list = os.listdir(path)
file_list_excel = [file for file in file_list if file.endswith(".xlsx")]
macro_file_name = []
result_file_name = []
eV_list = []
# 엑셀파일 이름 별로 저장할 매크로파일 이름

# 하모닉 순서대로 엑셀파일 이름 읽어오기
for excel_name in file_list_excel:
    #macro_file_name.append(excel_name.replace('xlsx','mac'))
    macro_file_name.append(excel_name.replace('.xlsx', ''))
    result_file_name.append(excel_name.replace('.xlsx', ''))
    # 엑셀파일이름이 harmonic number_XXXeV.xlsx 여서 _와 eV사이의 eV 값을 뺴내기 위함
    regex = re.compile('{}(.*){}'.format(re.escape('_'), re.escape('.x')))
    eV = regex.findall(excel_name)
    eV_list.append(eV[0])

#print(sorted(result_file_name,key=float))
excel_file_name_sorted_by_harmonicN = sorted(result_file_name,key=float)
eV_list_sorted_by_harmonicN = sorted(eV_list,key=float)
macro_file_name_sorted_by_harmonicN = sorted(macro_file_name,key=float)
result_file_name_sorted_by_harmonicN = sorted(macro_file_name,key=float)
print(excel_file_name_sorted_by_harmonicN)
print(macro_file_name_sorted_by_harmonicN)
print(result_file_name_sorted_by_harmonicN)
print(eV_list_sorted_by_harmonicN)

hadd_script_manual = open("hadd.txt","a")
hadd_script_manual.write("hadd all.root ")
for i, name in enumerate(result_file_name_sorted_by_harmonicN):
    #print(name)
    hadd_script_manual.write("{}.root ".format(macro_file_name_sorted_by_harmonicN[i]))
hadd_script_manual.close()

# photon portion 정할 excel 읽고 값 저장하기
photon_flux_raw_data = pd.read_excel("./../10m_20keV_1eV_5x3mm.xlsx")

Flux_Photon_data = pd.DataFrame.to_numpy(photon_flux_raw_data)
Flux_photon = []

for i in range(19992):
    Flux_photon.append(Flux_Photon_data[i][1])

Flux_photon = np.array(Flux_photon)
Flux_photon = Flux_photon/Flux_photon.sum()

photon_X_range = np.linspace(10, 20000, 19992)
LinInterp_Flux_photon  = interp1d(photon_X_range, Flux_photon, kind = 'linear')
LinInterp_Flux_photon_X = np.linspace(10, 20000, 19992)
LinInterp_Flux_photon_Y = LinInterp_Flux_photon(LinInterp_Flux_photon_X)
#print(LinInterp_Flux_photon(30))

# ------------------- Photon cumulated distribution ----------------------
N_photon_cumulate = []
tmp_X = np.linspace(10, 20000, 19992)
LinInterp_Flux_photon_Y_cum = LinInterp_Flux_photon_Y/LinInterp_Flux_photon_Y.sum()

tmp_variable = 0
for xi, ni in zip(tmp_X, LinInterp_Flux_photon_Y ):
    tmp_variable += ni
    N_photon_cumulate.append(tmp_variable)
N_photon_cumulate = np.array(N_photon_cumulate)

# Photon cumulated distribution interp. -------------------

photon_cumulative_X = np.linspace(10, 20000, 19992)
photon_cumulative_interp  = interp1d(photon_cumulative_X, N_photon_cumulate, kind = 'linear')
photon_cum_X = np.linspace(10, 20000, 19992)
photon_cum_Y = photon_cumulative_interp(photon_cum_X)
#print(photon_cumulative_interp(4860))
# Getting sum between 30 eV interval using photon interp.


tmp_sum = 0
interval_harmonic = 15
harmonicN_portion = []
for harmonic_index in range(163):
    harmonic_index_min = 30 * (harmonic_index + 1) - interval_harmonic
    harmonic_index_max = 30 * (harmonic_index + 1) + interval_harmonic
    #print(round(photon_cumulative_interp(harmonic_index_max)-photon_cumulative_interp(harmonic_index_min),6))
    harmonicN_portion.append(round(photon_cumulative_interp(harmonic_index_max)-photon_cumulative_interp(harmonic_index_min),5))

    #print("{} harmonic index min--> {}".format(harmonic_index, harmonic_index_min))
    #print("{} harmonic index max--> {}".format(harmonic_index, harmonic_index_max))
#harmonicN_portion = np.array(harmonicN_portion)
#print(photon_cumulative_interp(19992))

Total_Number_Of_Photon = float(4234993385100000000)
# 4.23E18 // 10 eV--> 20 keV 까지의 photon flux 합 ( bin size 1 eV )
# 10 eV --> ~ 4800 eV 까지의 합은 약 2.2E18 이라 절반이다.
# 다만 포톤의 개수가 절반이고, 생성된 전자의 수는 10eV-->4800eV까지가 약 80%, 4800eV-->20000eV가 약 20%
# 시간적 한계떄문에 4800eV까지 시뮬레이션 진행
#scale_down = 10000000000 # 1.E10
scale_down = 100000000 # 1.E8
#scale_down = 1000000000 # 1.E9
Scale_down_Total_Number_Of_Photon = int(Total_Number_Of_Photon/scale_down)



#---- 매크로파일 만드는 부분 시작
run_script_manual = open("RunScript0719.txt","a")
line_jumper = 0
for mac_num, FD_harmonic in enumerate(macro_file_name_sorted_by_harmonicN):
    #macro_file = open("myscrip.mac","w")
    macro_file = open("{}.mac".format(macro_file_name_sorted_by_harmonicN[mac_num]), "w")

    #df = pd.read_excel("1_30eV.xlsx")
    df = pd.read_excel("{}.xlsx".format(FD_harmonic))

    FD_raw_data_XY = pd.DataFrame.to_numpy(df)
    # X, Y, Height(or Weight) : array
    FD_raw_data_X = []
    FD_raw_data_Y = []
    FD_raw_data_H = []
    FD_raw_data_tmp_var = []
    # weight의 2-dim array를 저장할 변수
    FD_raw_data_Bincontent = []

    for index, data in enumerate(FD_raw_data_XY):
        # 첫 row는 단위 입력되어있음
        if index < 1 :
            continue
        FD_raw_data_X.append(data[0])
        FD_raw_data_Y.append(data[1])
        FD_raw_data_H.append(float(data[2]))
        # H(weight)의 2-dim array
        FD_raw_data_tmp_var.append(float(data[2]))
        if index % 501 == 0 :
            FD_raw_data_Bincontent.append(FD_raw_data_tmp_var)
            #    #print(FD_raw_data_Bincontent)
            FD_raw_data_tmp_var = []

    FD_raw_data_Bincontent = np.array(FD_raw_data_Bincontent)
    #print(FD_raw_data_Bincontent.shape)
    #print(FD_raw_data_H)

    FD_raw_data_X = np.array(FD_raw_data_X)
    FD_raw_data_Y = np.array(FD_raw_data_Y)
    FD_raw_data_H = np.array(FD_raw_data_H)
    #print("X_shape --> ",FD_raw_data_X.shape)
    #print("Y_shape --> ",FD_raw_data_Y.shape)
    #print("H_shape --> ",FD_raw_data_H.shape)
    # 스펙트라 raw data, Xbins = 501 / Ybins = 301, 매우 촘촘함
    interp2d_Xrange = np.linspace(-2.5,2.5,501)
    interp2d_Yrange = np.linspace(-1.5,1.5,301)

    # 내가 2D interp. 로 얻고자하는 XY mesh 설정 지금은 ( 50 x 30 )
    interp2d_X = np.linspace(-2.5,2.5,50)
    interp2d_Y = np.linspace(-1.5,1.5,30)
    interp2d_X = np.array(interp2d_X)
    interp2d_Y = np.array(interp2d_Y)

    Xi = []
    Xi = np.array(Xi)
    Yi = []
    Yi = np.array(Yi)
    # 스펙트라 데이터 저장형식이 Y : -1.5 mm 고정, X : -2.5 --> 2.5 mm 한 루프돌고 Y : -1.49 mm고정 X : -2.5 --> 2.5 mm 돌고반복
    # 라인스캔
    for x1 in range(30):
        # X는 -2.5 ~  2.5 mm 슬라이싱한 array 반복
        Xi = np.append(Xi, interp2d_X)
    for y1 in range(30):
        for looping in range(50):
            # X를 스캔할 동안 Y는 고정이여야 함
            Yi = np.append(Yi, interp2d_Y[y1])
    #print(Xi.shape)
    #print(Yi)

    # 2d interp.
    FD_interp2d = interp2d(interp2d_Xrange, interp2d_Yrange, FD_raw_data_Bincontent, kind='cubic')
    FD_interp2d_H = FD_interp2d(interp2d_X, interp2d_Y)
    FD_interp2d_H = np.array(FD_interp2d_H)
    #print(FD_interp2d_H.shape)
    #print(FD_interp2d_H.flatten().shape)

    # 모든 지점의 weight 값의 합 구해놓음
    scale_down_variable = 10000000000 # 1.0E10
    flux_sum = 0
    tmp_loop1 = 0
    for yi in range(30):
        for xi in range(50):
            #flux_sum += float(FD_interp2d_H.flatten()[tmp_loop1]/scale_down_variable)
            flux_sum += float(FD_interp2d_H.flatten()[tmp_loop1])
            tmp_loop1 += 1
    flux_sum_int = int(flux_sum/scale_down_variable)
    #print(flux_sum)
    #print(flux_sum_int)
    #print(int(flux_sum))

    # 매크로 들어갈 내용 시작
    macro_file.write("/tracking/verbose 0\n")
    macro_file.write("/control/verbose 0\n")
    macro_file.write("/run/verbose 2\n")
    macro_file.write("/run/initialize\n")
    macro_file.write("/process/list\n")

    # 2d interp. 는 2 dim array (n,m) --> flatten() --> (n*m,) 각 요소 끄집어 내기위한 변수
    loop_for_Weight = 0
    # y 값 -1.5 ~ 1.5를 30으로 나눔
    delta = 0.1
    # 한 라인 스캔 끝날 때 마다 delta * number --> 이 number
    slicing_y = 0
    for yi in range(30):
        for xi in range(50):
            #print("X-->",Xi[xi],"|| Y-->", Yi[50*yi+yi],"|| H --> ",FD_interp2d_H.flatten()[xi*yi+xi])
            if yi == 0 and xi == 0:
                #print("/gps/source/intensity ",int(FD_interp2d_H.flatten()[xi*yi+xi]/flux_sum))
                macro_file.write("/gps/source/intensity {}\n".format(float(FD_interp2d_H.flatten()[loop_for_Weight]/flux_sum)))
                macro_file.write("/gps/pos/type Plane\n")
                macro_file.write("/gps/pos/shape Square\n")
                macro_file.write("/gps/pos/halfx 0.05 mm\n")
                macro_file.write("/gps/pos/halfy 0.05 mm\n")
                #print("/gps/pos/centre {} {} {} mm".format(round(Xi[xi],6), round(Yi[50*yi + yi],4), -149))
                macro_file.write("/gps/pos/centre {} {} {} mm\n".format(round(Xi[xi],6) , round(-1.45 + delta*slicing_y,4), -149))
                macro_file.write("/gps/particle gamma\n")
                macro_file.write("/gps/energy {} eV\n".format(eV_list_sorted_by_harmonicN[mac_num]))
                #print("/gps/pos/type Beam")
                macro_file.write("/gps/direction 0 0.0 1.0\n")
                loop_for_Weight += 1
            else:
                #print("/gps/source/add ",int(FD_interp2d_H.flatten()[xi*yi+xi]/flux_sum))
                macro_file.write("/gps/source/add {}\n".format(float(FD_interp2d_H.flatten()[loop_for_Weight]/flux_sum)))
                macro_file.write("/gps/pos/type Plane\n")
                macro_file.write("/gps/pos/shape Square\n")
                macro_file.write("/gps/pos/halfx 0.05 mm\n")
                macro_file.write("/gps/pos/halfy 0.05 mm\n")
                #print("/gps/pos/centre {} {} {} mm".format(round(Xi[xi],6), round(Yi[50*yi + yi],4), -149))
                macro_file.write("/gps/pos/centre {} {} {} mm\n".format(round(Xi[xi], 6), round(-1.45 + delta * slicing_y,4), -149))
                macro_file.write("/gps/particle gamma\n")
                macro_file.write("/gps/energy {} eV\n".format(eV_list_sorted_by_harmonicN[mac_num]))
                #print("/gps/pos/type Beam")
                macro_file.write("/gps/direction 0 0.0 1.0\n")
                loop_for_Weight += 1
        slicing_y += 1

    macro_file.write("/analysis/setFileName {}\n".format(result_file_name_sorted_by_harmonicN[mac_num]))
    macro_file.write("/analysis/h2/setX 0 100 -5 5 mm\n")
    macro_file.write("/analysis/h2/setY 0 60 -3 3 mm\n")
    macro_file.write("/analysis/h2/setXaxis 0 X[mm]\n")
    macro_file.write("/analysis/h2/setYaxis 0 Y[mm]\n")

    macro_file.write("/analysis/h2/setX 1 100 -5 5 mm\n")
    macro_file.write("/analysis/h2/setY 1 60 -3 3 mm\n")
    macro_file.write("/analysis/h2/setXaxis 1 X[mm]\n")
    macro_file.write("/analysis/h2/setYaxis 1 Y[mm]\n")

    macro_file.write("/analysis/h1/set 2 1000 -10 10 mm\n")
    macro_file.write("/analysis/h1/setXaxis 2 X[mm]\n")
    macro_file.write("/analysis/h1/setYaxis 2 Entries\n")

    macro_file.write("/analysis/h1/set 3 1000 -10 10 mm\n")
    macro_file.write("/analysis/h1/setXaxis 3 Y[mm]\n")
    macro_file.write("/analysis/h1/setYaxis 3 Entries\n")

    macro_file.write("/analysis/h1/set 4 1000 -10 10 mm\n")
    macro_file.write("/analysis/h1/setXaxis 4 X[mm]\n")
    macro_file.write("/analysis/h1/setYaxis 4 Entries\n")

    macro_file.write("/analysis/h1/set 5 1000 -10 10 mm\n")
    macro_file.write("/analysis/h1/setXaxis 5 Y[mm]\n")
    macro_file.write("/analysis/h1/setYaxis 5 Entries\n")

    macro_file.write("/analysis/h1/set 6 1000 0 5000 eV\n")
    macro_file.write("/analysis/h1/setXaxis 6 eV\n")
    macro_file.write("/analysis/h1/setYaxis 6 Entries\n")

    macro_file.write("/analysis/h1/set 7 1000 0 5000 eV\n")
    macro_file.write("/analysis/h1/setXaxis 7 eV\n")
    macro_file.write("/analysis/h1/setYaxis 7 Entries\n")

    #macro_file.write("/run/printProgress {}\n".format(int(flux_sum_int/100)))
    #macro_file.write("/run/beamOn {}\n".format(int(flux_sum_int)))
    Number_Of_PhotonBeam = int(Scale_down_Total_Number_Of_Photon * harmonicN_portion[mac_num])

    print("Total Number Of Photon : ", Total_Number_Of_Photon)
    print("scale_down_amount : ", scale_down)
    print("Portion of {} eV when total event sum 1 : {}".format(eV_list_sorted_by_harmonicN[mac_num], float(harmonicN_portion[mac_num])))
    print("{} eV event, applying N-harmonic portion  --> {} events".format(eV_list_sorted_by_harmonicN[mac_num], Number_Of_PhotonBeam))

    macro_file.write("/run/printProgress {}\n".format(int(Number_Of_PhotonBeam/100)))
    macro_file.write("/run/beamOn {}\n".format(int(Number_Of_PhotonBeam)))

    print("----------------------------------------------------------")
    macro_file.close()
    if line_jumper !=0 and line_jumper % 20 == 0 :
        run_script_manual.write("exampleB1.exe {}.mac && \n".format(macro_file_name_sorted_by_harmonicN[mac_num]))
    else:
        run_script_manual.write("exampleB1.exe {}.mac && ".format(macro_file_name_sorted_by_harmonicN[mac_num]))
    line_jumper+=1
run_script_manual.close()

    #plt.hist2d(FD_raw_data_X, FD_raw_data_Y, bins=[501,301], weights=FD_raw_data_H)
    #plt.xlabel('x [mm]')
    #plt.ylabel('y [mm]')
    #plt.figure()

    #plt.hist2d(Xi, Yi, bins=[50,30], weights=FD_interp2d_H.flatten())
    ##plt.xlabel('x [mm]')
    #plt.ylabel('y [mm]')
    #plt.figure()
#plt.show()
