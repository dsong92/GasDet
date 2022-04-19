import numpy as np
from scipy.interpolate import interp1d
from matplotlib import pyplot as plt
import pandas as pd

df = pd.read_excel("PBPM_BeamProfile_ForPython.xlsx")
PhotonEnergyCol = df["PhotonEnergy"]
FluxCol = df["Flux"]
np_photE = pd.DataFrame.to_numpy(df)
#print(np[1][1])

x_photE = []
y_flux = []
for i in range(500):
    x_photE.append(np_photE[i][0])
    y_flux.append(np_photE[i][1])

x_electron_prob = [ 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 300, 400,
                    500, 600, 630, 660, 690, 700, 800, 900, 920, 930, 940, 950, 960, 1000]
y_electron_prob = [ 0, 3.19774e-05, 6.79932e-06, 2.6081e-06, 1.41218e-06, 9.383e-07, 4.11598e-06, 5.09777e-05, 3.45015e-05, 1.42452e-05
    ,6.61716e-06, 3.30396e-06, 1.81654e-06, 1.15346e-06, 1.48082e-06, 1.44936e-06, 1.65682e-06, 1.77562e-06, 1.82402e-06
    ,1.94172e-06, 2.21826e-06, 2.30934e-06, 2.32298e-06, 2.35224e-06, 1.91092e-06, 1.45178e-06, 1.13124e-06, 1.05886e-06
    ,1.00276e-06, 4.12346e-06, 6.64444e-06, 4.35248e-06, 3.33454e-06, 3.26832e-06, 3.58336e-06, 3.44542e-06, 3.43464e-06
    ,3.39064e-06, 3.21838e-06 ]

fig, ax1 = plt.subplots()

#linear, quadratic
fl  = interp1d(x_photE, y_flux, kind = 'linear')
fl2 = interp1d(x_electron_prob, y_electron_prob, kind = 'linear')

xint = np.linspace(min(x_photE), max(x_photE), 1000)
yintl = fl(xint)
xint2 = np.linspace(min(x_electron_prob), max(x_electron_prob), 1000)
yintl2 = fl2(xint2)

#plt.plot(xint, yintl, color = 'green', label="Linear Interpolation", linewidth=2)
ax1.plot(xint, yintl, color='green', label="Linear Interpolation", linewidth=2)
ax1.plot(x_photE, y_flux,'o', label="Photon Energy at PBPM")    # 값의 위치를 점으로
line1 = ax1.plot(x_photE, y_flux,'o', color = "steelblue", label="Photon Energy at PBPM(Simul.)")
line2 = ax1.plot(xint, yintl, color='green', label="Linear Interp. - $E_{photon}$ ", linewidth=2)

ax2 = ax1.twinx()
ax2.plot(xint2, yintl2, color = 'red', label="Linear Interpolation", linewidth=2)
ax2.plot(x_electron_prob, y_electron_prob,'o', label="")
line3 = ax2.plot(x_electron_prob, y_electron_prob,'o', label="Ratio of $e^{-}_{phot}$ per photon$\cdot$cm(Simul.)")
line4 = ax2.plot(xint2, yintl2, color = 'red', label="Linear Interp. - Ratio of $e^{-}_{phot}$ per photon$\cdot$cm", linewidth=2)
lines = line1 + line2 + line3 + line4

labels= [l.get_label() for l in lines]
ax1.legend(lines, labels)

ax1.set_yscale("log")
ax1.set_xlabel("Photon Energy[eV]")
ax1.set_ylabel("Photon Flux", color='green')

ax2.set_yscale("log")
ax2.set_ylabel("Ratio of photoelectron per photon$\cdot$cm", color='red')

'''
x_Sampling = []
y_Expected_flux = []
array_for_excel = np.empty((0, 2))

for i in range(16398, 1000000, 32696):
    #print(fl(i/1000))
    x_Sampling.append(i/1000)
    y_Expected_flux.append(fl(i/1000))
    array_for_excel = np.append(array_for_excel, np.array([[i/1000, fl(i/1000)]]), axis=0)


for i in range(10, 1001, 1):
    #print(fl(i/1000))
    x_Sampling.append(i)
    y_Expected_flux.append(fl(i))
    array_for_excel = np.append(array_for_excel, np.array([[i, fl(i)]]), axis=0)

df = pd.DataFrame(array_for_excel, columns=['PhotonEnergy', 'Flux'])
df.to_excel('testAll.xlsx')
'''

'''
x_Sampling2 = []
y_Expected_flux2 = []
array_for_excel2 = np.empty((0, 2))

for i in range(10, 1001, 1):
    #print(fl(i/1000))
    x_Sampling2.append(i)
    y_Expected_flux2.append(fl2(i))
    array_for_excel2 = np.append(array_for_excel2, np.array([[i, fl2(i)]]), axis=0)

df = pd.DataFrame(array_for_excel2, columns=['PhotonEnergy', 'RatioOfGenElec'])
df.to_excel('test_prob.xlsx')
'''




x_Sampling3 = []
y_Sampling3 = []
array_for_excel3 = np.empty((0, 2))
array_for_excel4 = np.empty((0, 2))
array_for_excel5 = np.empty((0, 2))

for i in range(10, 1000, 10):
    x_Sampling3.append(i)
    y_Sampling3.append(fl(i)*fl2(i))
    array_for_excel3 = np.append(array_for_excel3, np.array([[i, fl(i)*fl2(i)]]), axis=0)
    if(i % 20 == 0):
        array_for_excel4 = np.append(array_for_excel4, np.array([[i, fl(i)*fl2(i)]]), axis=0)
    if(i % 30 == 0):
        array_for_excel5 = np.append(array_for_excel5, np.array([[i, fl(i)*fl2(i)]]), axis=0)

df3 = pd.DataFrame(array_for_excel3, columns=['PhotonEnergy', 'PhotonFlux*RationOfElectron'])
df4 = pd.DataFrame(array_for_excel4, columns=['PhotonEnergy', 'PhotonFlux*RationOfElectron'])
df5 = pd.DataFrame(array_for_excel5, columns=['PhotonEnergy', 'PhotonFlux*RationOfElectron'])

writer = pd.ExcelWriter('result.xlsx', engine='xlsxwriter')
df3.to_excel(writer, sheet_name='Sheet1', index=False)
df4.to_excel(writer, sheet_name='Sheet2', index=False)
df5.to_excel(writer, sheet_name='Sheet3', index=False)
writer.save()

#plt.plot(x_Sampling3, y_Sampling3, 'o', label="10eV interval")


plt.title('Photon Flux vs. $E_{photon}$ and Ratio of $e^{-}_{phot}$ per incident photon$\cdot$cm')
plt.show()


