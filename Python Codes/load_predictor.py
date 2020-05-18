import pandas as pd 
import numpy as np # matplotlib and seaborn are used for plotting graphs 
import matplotlib.pyplot as plt 
import seaborn
path = '~/Documents/FinalYearProject/SolarPanel_PredictiveAnalysis/DataSet/load_data_final.csv'
Df= pd.read_csv(path)
Df.head()
Df.shape
Df['S_3'] = Df['Average Power'].shift(1).rolling(window=3).mean() 
Df['S_9']= Df['Average Power'].shift(1).rolling(window=9).mean() 
Df.head()
Df =Df.dropna()
X = Df[['S_3','S_9']] 
X.head()
y = Df['Average Power']
y.head()
X.shape, y.shape
t=.8 
t =(t*len(Df)) 
# Train dataset 
t=int(t)
X_train = X[:t] 
y_train = y[:t]  
# Test dataset 
X_test = X[t:] 
y_test = y[t:]
X_train.shape, y_train.shape, X_test.shape, y_test.shape
from sklearn.linear_model import LinearRegression
model = LinearRegression()
model.fit(X_train, y_train)
# Import the model we are using
from sklearn.ensemble import RandomForestRegressor
# Instantiate model with 1000 decision trees
model= RandomForestRegressor(n_estimators = 1000, random_state = 42,verbose=2)
# Train the model on training data
model.fit(X_train,y_train);
model.score(X_train, y_train)
pred_power = model.predict(X_test)
accuracy = model.score(X_train, y_train)*100
error = 100-accuracy
print("test_error % = {}".format(error))
accuracy= 100 - error
print("test_accuracy %={}".format(accuracy))
predicted_power = pd.DataFrame(pred_power,index=y_test.index,columns = ['Average Power'])
predicted_power.plot(figsize=(100,50))  
y_test.plot()  
plt.legend(['predicted_power','actual_power'])  
plt.ylabel("Average Power")  
plt.show()
