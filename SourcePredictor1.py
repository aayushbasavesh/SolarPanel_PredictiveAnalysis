import pandas as pd
import numpy as np

from pandas import read_csv
from sklearn.model_selection import train_test_split
from sklearn.metrics import classification_report
from sklearn.metrics import confusion_matrix
from sklearn.metrics import accuracy_score
from sklearn.linear_model import LogisticRegression


data= pd.read_csv('/home/aayush/Documents/FinalYearProject/SolarPanel_PredictiveAnalysis/FinalDataSet.csv')
data = data[data['Power']!=0]

y=data["Power"]
y=np.array(y)
y= y.reshape(-1,1)
y.shape

x=data.drop(columns=["Power","datetime","Voltage","Current"])
x.head()

from sklearn.model_selection import train_test_split 
from sklearn.linear_model import LinearRegression
X_train, X_test, y_train, y_test = train_test_split(x, y, test_size=0.25, random_state=1)
regressor = LinearRegression()  
regressor.fit(X_train, y_train)

y_pred = regressor.predict(X_train)

from sklearn.metrics import r2_score
r2_score(y_train,y_pred)

import math
error =[np.power((b-a),2) for (a,b) in zip(y_pred,y_train)]
error0 = np.sum(error)
error = math.sqrt(error0)
error = (error/len(y_test))*100

print("training error % = {}".format(error))
accuracy = 100 - error
print("training Accuracy % = {}".format(accuracy))

y_test_pred = regressor.predict(X_test)

r2_score(y_test,y_test_pred)
error =[np.power((b-a),2) for (a,b) in zip(y_test_pred,y_test)]
error0 = np.sum(error)
error = math.sqrt(error0)
error = (error/len(y_test))*100

print("Test error % = {}".format(error))
accuracy = 100 - error
print("Test Accuracy % = {}".format(accuracy))

from sklearn.neighbors import KNeighborsRegressor
errort=[]
for i in range(1,100):
  knn=KNeighborsRegressor(algorithm='brute',n_neighbors=i)
  knn.fit(X_train,y_train)
  pred_i=knn.predict(X_test)
  error =[np.power((b-a),2) for (a,b) in zip(y_test_pred,y_test)]
  error0 = np.sum(error)
  error = math.sqrt(error0)
  error = (error/len(y_test))*100

  accuracy = 100 - error
  
  errort.append(accuracy)
import matplotlib.pyplot as plt
plt.figure(figsize=(12,6))
plt.plot(range(1,100),errort,color='red',linestyle='dashed',marker='o',markerfacecolor='blue',markersize=10)
plt.title("Error vs K")
plt.xlabel('K val')
plt.ylabel('Mean error')

#print(error.index(min(error))+1)
knn=KNeighborsRegressor(algorithm='brute',n_neighbors=4)
knn.fit(X_train,y_train)

y_pred=knn.predict(X_train)
y_pred.shape,y_train.shape

from sklearn.metrics import r2_score
r2_score(y_train,y_pred)

import math
error =[np.power((b-a),2) for (a,b) in zip(y_pred,y_train)]
error0 = np.sum(error)
error = math.sqrt(error0)
error = (error/len(y_test))*100

print("training error % = {}".format(error))
accuracy = 100 - error
print("training Accuracy % = {}".format(accuracy))

y_test_pred = knn.predict(X_test)

r2_score(y_test,y_test_pred)

error =[np.power((b-a),2) for (a,b) in zip(y_test_pred,y_test)]
error0 = np.sum(error)
error = math.sqrt(error0)
error = (error/len(y_test))*100

print("Test error % = {}".format(error))
accuracy = 100 - error
print("Test Accuracy % = {}".format(accuracy))

