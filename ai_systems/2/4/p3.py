#!/usr/bin/env python3

import pandas as pd
from sklearn.ensemble import RandomForestClassifier, StackingClassifier
from sklearn.impute import SimpleImputer
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import accuracy_score
from sklearn.model_selection import train_test_split
from sklearn.neighbors import KNeighborsClassifier
from sklearn.pipeline import make_pipeline
from sklearn.preprocessing import OneHotEncoder, StandardScaler
from sklearn.svm import SVC


def preprocess_data(train_path, test_path):
    train = pd.read_csv(train_path)
    test = pd.read_csv(test_path)

    y_train = train["Survived"]
    X_train = train.drop(columns=["Survived", "PassengerId", "Name", "Ticket", "Cabin"])

    test_ids = test["PassengerId"]
    X_test = test.drop(columns=["PassengerId", "Name", "Ticket", "Cabin"])

    num_cols = ["Age", "Fare", "SibSp", "Parch", "Pclass"]
    cat_cols = ["Sex", "Embarked"]

    num_imputer = SimpleImputer(strategy="median")
    X_train[num_cols] = num_imputer.fit_transform(X_train[num_cols])
    X_test[num_cols] = num_imputer.transform(X_test[num_cols])

    cat_imputer = SimpleImputer(strategy="most_frequent")
    X_train[cat_cols] = cat_imputer.fit_transform(X_train[cat_cols])
    X_test[cat_cols] = cat_imputer.transform(X_test[cat_cols])

    X_train = pd.get_dummies(X_train, columns=cat_cols, drop_first=True)
    X_test = pd.get_dummies(X_test, columns=cat_cols, drop_first=True)
    X_test = X_test.reindex(columns=X_train.columns, fill_value=0)

    return X_train, y_train, X_test, test_ids


X, y, X_test_final, test_ids = preprocess_data("titanic_train.csv", "titanic_test.csv")

X_train, X_val, y_train, y_val = train_test_split(
    X, y, test_size=0.2, random_state=42, stratify=y
)

base_estimators = [
    ("rf", RandomForestClassifier(n_estimators=100, random_state=42)),
    ("svc", make_pipeline(StandardScaler(), SVC(probability=True, random_state=42))),
    ("knn", make_pipeline(StandardScaler(), KNeighborsClassifier(n_neighbors=5))),
]

stacking_model = StackingClassifier(
    estimators=base_estimators,
    final_estimator=LogisticRegression(random_state=42),
    cv=5,
    n_jobs=-1,
)

stacking_model.fit(X_train, y_train)

val_predictions = stacking_model.predict(X_val)
val_accuracy = accuracy_score(y_val, val_predictions)

print(f"Качество классификации (Accuracy) на валидационной выборке: {val_accuracy:.4f}")
