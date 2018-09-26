#include <QtWidgets>
#include "memorytest.h"

// List of allowed IC type
const QStringList memoryType  = {"V4", "L06B", "L95B"};

// List of allowed fileextension
const QStringList extensions  = {"*.xlsm", "*.xlsx", "*.xls"};

// Paramters of lot name
const QStringList productCode   = {"N"};
const QStringList crystalCode   = {"V", "L", "B"};
const QStringList countStack    = {"1", "2", "4", "8"};
const QStringList status        = {"P", "E", "Q", "R"};

Window::Window(QWidget *parent):QWidget(parent)
{
    // Create parent window
    QGridLayout *mainLayout = new QGridLayout(this);

    // Define header and screen size
    setWindowTitle(tr("Memory test"));
    const QRect screenGeometry = QApplication::desktop()->screenGeometry(this);
    resize(screenGeometry.width() / 2, screenGeometry.height() / 3);

    // Define temporary list of used ICs. It needs for initialization of combobox
    fileComboBox = createComboBox(memoryType);

    // Create combobox with initial value which informs about some action
    configComboBox = createComboBox({"Need to set path to config files to get list of the files"});

    // Create lines
    lineTestCase        = showLine();
    lineType            = showLine();
    lineCountStack      = showLine();
    lineConfig          = showLine();
    lineSamples         = showLine();

    // Create buttons for GUI
    QPushButton *validateButton = new QPushButton(tr("Validate"));
    connect(validateButton, &QAbstractButton::clicked, this, &Window::getTestCase);

    QPushButton *browseButton = new QPushButton(tr("Browse"));
    connect(browseButton, &QAbstractButton::clicked, this, &Window::browse);

    QPushButton *launchTest = new QPushButton(tr("Run"));
    connect(launchTest, &QAbstractButton::clicked, this, &Window::getInputParameters);

    // Add graphic elements into the parent window
    mainLayout->addWidget(new QLabel("Set a path to the config"), 0, 0);
    mainLayout->addWidget(lineConfig, 0, 1);
    mainLayout->addWidget(browseButton, 0, 2);

    mainLayout->addWidget(new QLabel("Set test case"), 1, 0);
    mainLayout->addWidget(lineTestCase, 1, 1);
    mainLayout->addWidget(validateButton, 1, 2);
    lineTestCase->setPlaceholderText("You can use UP, low or mIX case");

    mainLayout->addWidget(new QLabel("Set memory type"), 3, 0);
    mainLayout->addWidget(fileComboBox, 3, 1);

    mainLayout->addWidget(new QLabel("Set config files"), 4, 0);
    mainLayout->addWidget(configComboBox, 4, 1);

    mainLayout->addWidget(new QLabel("Set count of crystal"), 5, 0);
    mainLayout->addWidget(lineCountStack, 5, 1);

    mainLayout->addWidget(new QLabel("Set count of samples"), 6, 0);
    mainLayout->addWidget(lineSamples, 6, 1);

    mainLayout->addWidget(launchTest, 7, 0);

}

QComboBox *Window::createComboBox(const QStringList &list)
{
    /*!
        Function creates box with multiple values and shows their
    */

    QComboBox *comboBox = new QComboBox;
    comboBox->addItems(list);
    comboBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    return comboBox;
}

QLineEdit *Window::showLine()
{
    /*!
        Creates and returns line for typing
    */

    QLineEdit *lineEdit = new QLineEdit;
    lineEdit->setFocus();
    return lineEdit;
}

void Window::browse()
{
    /*!
        Helps set path to a folder
    */

    // Create variable to store input path
    QString directory = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Find Files")));

    // Show path to input directory if path is not empty
    if (!directory.isEmpty()) {
        lineConfig->setText(directory); // Show path in GUI
        findFilesInDirectory(directory);
    }
}

void Window::updateConfigBox(QComboBox *configComboBox, QStringList listValues)
{
    /*!
        Updates elements from filtered list. Informs about list of elements
    */

    // Remove any elements in the box
    configComboBox->clear();

    // Show all values from predefined list
    for (qint8 i = 0; i < listValues.size(); i++)
        configComboBox->addItem(listValues[i]);
}

void Window::findFilesInDirectory(QString pathToDirectory)
{
    /*!
        Filters files in a directory based on file extension
    */

    // Storage for filtered files
    QStringList temporaryList = {""};

    // Path to directory
    QDir folder(pathToDirectory);

    // Filter files in folder using predefined extensions
    folder.setNameFilters(QStringList() << extensions[0] << extensions[1] << extensions[2]);
    temporaryList = folder.entryList();

    // Set files for combobox with required extensions
    updateConfigBox(configComboBox, temporaryList);
}

static void updateComboBox(QComboBox *comboBox, QString inputValue)
{
    /*!
        Updates values in combobox
    */

    // Remove any elements in the box
    comboBox->clear();

    // Set type of IC based on input value
    QString usedType = "";
    if (inputValue.startsWith(productCode[0] + crystalCode[0]))
        usedType = memoryType[0];
    else if (inputValue.startsWith(productCode[0] + crystalCode[1]))
        usedType = memoryType[1];
    else if (inputValue.startsWith(productCode[0] + crystalCode[2]))
        usedType = memoryType[2];

    // Add value to the box
    comboBox->addItem(usedType);
    comboBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
}

void Window::getTestCase()
{
    /*!
        Checks input parameter has mapping to predefined template
    */

    // Get IC name from GUI
    QString inputValue = this->lineTestCase->text();

    // Change text case to upper
    inputValue = inputValue.toUpper();

    // Define variables for forming name of lot
    QString testCase        = "";
    QString tempStackValue  = "";
    QString countLots       = "[0-9]{4}"; // Allows use a number with 3 digits
    QString endOfLine       = "$";

    // Check of some input parameters and set parameters of test
    if (inputValue.startsWith(productCode[0] + crystalCode[0]))
    {
        // Form output string
        tempStackValue  = countStack[0];
        testCase        = productCode[0] + crystalCode[0] + tempStackValue + status[0];

    }
    else if (inputValue.startsWith(productCode[0] + crystalCode[1] + countStack[1]) )
    {
        // Form output string
        tempStackValue  = countStack[1];
        testCase        = productCode[0] + crystalCode[1] + tempStackValue + status[1];
    }
    else if (inputValue.startsWith(productCode[0] + crystalCode[1] + countStack[2]) )
    {
        // Form output string
        tempStackValue  = countStack[2];
        testCase        = productCode[0] + crystalCode[1] + tempStackValue + status[2];
    }
    else if (inputValue.startsWith(productCode[0] + crystalCode[2] + countStack[3]) )
    {
        // Form output string
        tempStackValue  = countStack[3];
        testCase        = productCode[0] + crystalCode[2] + tempStackValue + status[3];
    }

    // Create output string
    testCase = testCase + countLots + endOfLine;

    // Verify input name is consistent with required template
    QRegularExpression re(testCase);
    QRegularExpressionMatch match = re.match(inputValue);

    // Create bool variable that defines consistent between template and input from user
    bool hasMatch = match.hasMatch();
    if (hasMatch)
    {
        // Set text and block edit it
        lineCountStack->setText(tempStackValue);
        lineCountStack->setReadOnly(true);

        // Change color of line when it blocked
        QPalette *palette = new QPalette();
        palette->setColor(QPalette::Base,Qt::lightGray);
        palette->setColor(QPalette::Text,Qt::black);
        lineCountStack->setPalette(*palette);

        // Update widgets
        updateComboBox(fileComboBox, inputValue);
    }
    else
    {
        // Make boxes empty when  some errors in input string
        updateComboBox(fileComboBox, "");
        QStringList tempList = {""}; // Empty value will be show when no match with input
        updateConfigBox(configComboBox, tempList);
    }
}

void Window::getInputParameters()
{
    /*!
        Helps get all input parameters from UI and use their for other functions/methods
    */

    // Get values from UI
    QString uiLotNumber     = lineTestCase->text();
    QString uiMemoryType    = fileComboBox->currentText();
    QString uiCountStack    = lineCountStack->text();
    QString uiConfigPath    = lineConfig ->text();
    QString uiConfigName    = configComboBox->currentText();
    QString uiSamples       = lineSamples->text();

    // Verify no empty input parameters
    if (!uiLotNumber.isEmpty() && !uiMemoryType.isEmpty() && !uiConfigPath.isEmpty()
            && !uiCountStack.isEmpty() && !uiSamples.isEmpty() && uiSamples != "0")
    {
        // Output to console for testing purpose only!
        qInfo() << "LotNumber "         << uiLotNumber;
        qInfo() << "MemoryType "        << uiMemoryType;
        qInfo() << "ConfigDirectory "   << uiConfigPath;
        qInfo() << "CountStack "        << uiCountStack;
        qInfo() << "Samples "           << uiSamples;
        qInfo() << "Config name "       << uiConfigName;
    }
}
