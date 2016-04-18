#include "ImageSequence.h"

ImageSequence::ImageSequence(const std::string& sRootPath, const std::string& sPrefix, const std::string& sExtension, bool bIsColor) {
    init();
    m_sRootPath = sRootPath;
    m_sPrefix = sPrefix;
    m_sExtension = sExtension;
    m_bIsColor = bIsColor;
}

void ImageSequence::init(void) {
    m_sRootPath = "";
    m_sPrefix = "";
    m_sExtension = "bmp";
	m_iOffest = -1;
    m_iFirstNumber = 0;
    m_iPaddingLength = 4;
    m_cPaddingCharacter = '0';
    m_bIsColor = true;
}

std::string ImageSequence::makePath(void) {
    ++m_iOffest;
    return m_sRootPath + m_sPrefix + getSequenceNumberString() + "." + m_sExtension;
}

bool ImageSequence::readImage(cv::Mat& mImage) {
    std::string sPath = makePath();
    mImage = cv::imread(sPath, m_bIsColor ? cv::IMREAD_COLOR : cv::IMREAD_GRAYSCALE);
    return mImage.empty() ? false : true;
}

cv::Mat ImageSequence::readImage(void) {
    std::string sPath = makePath();
    return cv::imread(sPath, m_bIsColor ? cv::IMREAD_COLOR : cv::IMREAD_GRAYSCALE);
}

bool ImageSequence::operator>>(cv::Mat& mImage) {
    std::string sPath = makePath();
    mImage = cv::imread(sPath, m_bIsColor ? cv::IMREAD_COLOR : cv::IMREAD_GRAYSCALE);
    return mImage.empty() ? false : true;
}

bool ImageSequence::writeImage(const cv::Mat& mImage) {
    std::string sPath = makePath();
    return cv::imwrite(sPath, mImage);
}

bool ImageSequence::operator<<(const cv::Mat& mImage) {
    std::string sPath = makePath();
    return imwrite(sPath, mImage);
}

void ImageSequence::setAttribute(const Attribute attrbute, const std::string& sValue) {
    switch (attrbute) {
    case Attribute::EXTENSION:
        m_sExtension = sValue;
        break;
    case Attribute::PREFIX:
        m_sPrefix = sValue;
        break;
    case Attribute::ROOT_PATH:
        m_sRootPath = sValue;
        break;
    default:
        qDebug("ImageSequence::Error occur when setting string attribute");
        break;
    }
}

void ImageSequence::setAttribute(const Attribute attrbute, int iValue) {
    switch (attrbute) {
    case Attribute::FIRST_NUMBER:
        m_iFirstNumber = iValue;
        break;
    case Attribute::PADDING_LENGTH:
        m_iPaddingLength = iValue;
        break;
    case Attribute::OFFSET:
        m_iOffest = iValue;
		break;
    default:
        qDebug("ImageSequence::Error occur when setting integer attribute");
        break;
    }
}

void ImageSequence::setAttribute(const Attribute attrbute, char cValue) {
    switch (attrbute) {
    case Attribute::PADDING_CHARACTER:
        m_cPaddingCharacter = cValue;
        break;
    default:
        qDebug("ImageSequence::Error occur when setting character attribute");
        break;
    }
}

std::string ImageSequence::getSequenceNumberString(void) const {
    std::stringstream sSequenceNumber;
    sSequenceNumber << std::setfill(m_cPaddingCharacter)
                    << std::setw(m_iPaddingLength)
					<< (m_iFirstNumber + m_iOffest);
    return sSequenceNumber.str();
}
