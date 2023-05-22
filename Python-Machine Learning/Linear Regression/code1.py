import pandas as pd
import numpy as np
import pickle

from sklearn.pipeline import Pipeline
from sklearn.impute import SimpleImputer
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split
from sklearn.base import BaseEstimator, TransformerMixin
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error
from sklearn.preprocessing import OneHotEncoder
from sklearn.compose import ColumnTransformer


if __name__ == "__main__":
    ds = pd.read_pickle('appml-assignment1-dataset.pkl')
    features = ds['X']
    labels = ds['y']

    features['hour'] = pd.DatetimeIndex(features['date']).hour
    features['date'] = pd.DatetimeIndex(features['date']).day_of_week

    train_set, test_set = train_test_split(features, test_size=0.2, random_state=42)
    features_tr = train_set.copy()
    features_test = test_set.copy()

    train_set, test_set = train_test_split(labels, test_size=0.2, random_state=42)
    ytrain = train_set.copy()
    ytest = test_set.copy()

    num_pipeline = Pipeline([
        ('imputer', SimpleImputer(strategy="median")),
        ('std_scaler', StandardScaler()),
    ])

    num_attribs = list(features.select_dtypes(include=[np.number]))
    full_pipeline = ColumnTransformer([
        ("num", num_pipeline, num_attribs),
        ("day_hour", OneHotEncoder(), ["date", "hour"])
    ])

    xtrain = full_pipeline.fit_transform(features_tr)
    xtest = full_pipeline.transform(features_test)
    pickle.dump(full_pipeline, open("pipeline1.pkl", 'wb'))

    model=LinearRegression()
    model.fit(xtrain,ytrain)
    pickle.dump(model, open("model1.pkl", 'wb'))

    pred_test=model.predict(xtest)
    rmse_test=np.sqrt(mean_squared_error(ytest,pred_test))
    print(rmse_test)