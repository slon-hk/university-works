def load_data(train_size=0.7):
    iris = load_iris()
    X, y = iris.data, iris.target
    X = X[:, :3]
    X_train, X_test, y_train, y_test = train_test_split(
        X, y,
        train_size=train_size,
        random_state=42,
        stratify=y
    )
    return X_train, X_test, y_train, y_test

def convert_to_pandas(X, y):
    df = pd.DataFrame(X, columns=['sepal_length', 'sepal_width', 'petal_length'])
    target_series = pd.Series(y, name='species')
    return df, target_series

def explore_data(df, target_series):
    print(df.head())
    print(df.describe())
    print(target_series.value_counts())
    return None

def preprocess_data(X_train, X_test, scaler_type='standard'):
    if scaler_type == 'standard':
        scaler = StandardScaler()
    elif scaler_type == 'minmax':
        scaler = MinMaxScaler()
    elif scaler_type == 'robust':
        scaler = RobustScaler()
    else:
        return None

    X_train_scaled = scaler.fit_transform(X_train)
    X_test_scaled = scaler.transform(X_test)
    return X_train_scaled, X_test_scaled

def train_model(X_train, y_train, max_depth=5):
    """max_depth ограничивает глубину дерева: меньшие значения уменьшают переобучение, большие — повышают сложность модели."""
    model = DecisionTreeClassifier(max_depth=max_depth, random_state=42)
    model.fit(X_train, y_train)
    return model

def predict(model, X_test):
    return model.predict(X_test)

def evaluate(y_pred, y_test):
    """На точность повлияли: качество признаков, глубина дерева, тип предобработки и размер обучающей выборки."""
    score = accuracy_score(y_test, y_pred)
    return round(float(score), 3)
