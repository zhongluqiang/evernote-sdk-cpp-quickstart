/**
 * Autogenerated by Thrift Compiler (0.12.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#include "Limits_constants.h"

namespace evernote { namespace limits {

const LimitsConstants g_Limits_constants;

LimitsConstants::LimitsConstants() {
  EDAM_ATTRIBUTE_LEN_MIN = 1;

  EDAM_ATTRIBUTE_LEN_MAX = 4096;

  EDAM_ATTRIBUTE_REGEX = "^[^\\p{Cc}\\p{Zl}\\p{Zp}]{1,4096}$";

  EDAM_ATTRIBUTE_LIST_MAX = 100;

  EDAM_ATTRIBUTE_MAP_MAX = 100;

  EDAM_GUID_LEN_MIN = 36;

  EDAM_GUID_LEN_MAX = 36;

  EDAM_GUID_REGEX = "^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$";

  EDAM_EMAIL_LEN_MIN = 6;

  EDAM_EMAIL_LEN_MAX = 255;

  EDAM_EMAIL_LOCAL_REGEX = "^[A-Za-z0-9!#$%&'*+/=?^_`{|}~-]+(\\.[A-Za-z0-9!#$%&'*+/=?^_`{|}~-]+)*$";

  EDAM_EMAIL_DOMAIN_REGEX = "^[A-Za-z0-9-]*[A-Za-z0-9](\\.[A-Za-z0-9-]*[A-Za-z0-9])*\\.([A-Za-z]{2,})$";

  EDAM_EMAIL_REGEX = "^[A-Za-z0-9!#$%&'*+/=?^_`{|}~-]+(\\.[A-Za-z0-9!#$%&'*+/=?^_`{|}~-]+)*@[A-Za-z0-9-]*[A-Za-z0-9](\\.[A-Za-z0-9-]*[A-Za-z0-9])*\\.([A-Za-z]{2,})$";

  EDAM_VAT_REGEX = "^(AT)?U[0-9]{8}$|^(BE)?0?[0-9]{9}$|^(BG)?[0-9]{9,10}$|^(CY)?[0-9]{8}L$|^(CZ)?[0-9]{8,10}$|^(DE)?[0-9]{9}$|^(DK)?[0-9]{8}$|^(EE)?[0-9]{9}$|^(EL|GR)?[0-9]{9}$|^(ES)?[0-9A-Z][0-9]{7}[0-9A-Z]$|^(FI)?[0-9]{8}$|^(FR)?[0-9A-Z]{2}[0-9]{9}$|^(GB)?([0-9]{9}([0-9]{3})?|[A-Z]{2}[0-9]{3})$|^(HU)?[0-9]{8}$|^(IE)?[0-9]{7}[A-Z]{1,2}$|^(IT)?[0-9]{11}$|^(LT)?([0-9]{9}|[0-9]{12})$|^(LU)?[0-9]{8}$|^(LV)?[0-9]{11}$|^(MT)?[0-9]{8}$|^(NL)?[0-9]{9}B[0-9]{2}$|^(PL)?[0-9]{10}$|^(PT)?[0-9]{9}$|^(RO)?[0-9]{2,10}$|^(SE)?[0-9]{12}$|^(SI)?[0-9]{8}$|^(SK)?[0-9]{10}$|^[0-9]{9}MVA$|^[0-9]{6}$|^CHE[0-9]{9}(TVA|MWST|IVA)$";

  EDAM_TIMEZONE_LEN_MIN = 1;

  EDAM_TIMEZONE_LEN_MAX = 32;

  EDAM_TIMEZONE_REGEX = "^([A-Za-z_-]+(/[A-Za-z_-]+)*)|(GMT(-|\\+)[0-9]{1,2}(:[0-9]{2})?)$";

  EDAM_MIME_LEN_MIN = 3;

  EDAM_MIME_LEN_MAX = 255;

  EDAM_MIME_REGEX = "^[A-Za-z]+/[A-Za-z0-9._+-]+$";

  EDAM_MIME_TYPE_GIF = "image/gif";

  EDAM_MIME_TYPE_JPEG = "image/jpeg";

  EDAM_MIME_TYPE_PNG = "image/png";

  EDAM_MIME_TYPE_TIFF = "image/tiff";

  EDAM_MIME_TYPE_BMP = "image/bmp";

  EDAM_MIME_TYPE_WAV = "audio/wav";

  EDAM_MIME_TYPE_MP3 = "audio/mpeg";

  EDAM_MIME_TYPE_AMR = "audio/amr";

  EDAM_MIME_TYPE_AAC = "audio/aac";

  EDAM_MIME_TYPE_M4A = "audio/mp4";

  EDAM_MIME_TYPE_MP4_VIDEO = "video/mp4";

  EDAM_MIME_TYPE_INK = "application/vnd.evernote.ink";

  EDAM_MIME_TYPE_PDF = "application/pdf";

  EDAM_MIME_TYPE_DEFAULT = "application/octet-stream";

  EDAM_MIME_TYPES.insert("image/gif");
  EDAM_MIME_TYPES.insert("image/jpeg");
  EDAM_MIME_TYPES.insert("image/png");
  EDAM_MIME_TYPES.insert("audio/wav");
  EDAM_MIME_TYPES.insert("audio/mpeg");
  EDAM_MIME_TYPES.insert("audio/amr");
  EDAM_MIME_TYPES.insert("application/vnd.evernote.ink");
  EDAM_MIME_TYPES.insert("application/pdf");
  EDAM_MIME_TYPES.insert("video/mp4");
  EDAM_MIME_TYPES.insert("audio/aac");
  EDAM_MIME_TYPES.insert("audio/mp4");

  EDAM_INDEXABLE_RESOURCE_MIME_TYPES.insert("application/msword");
  EDAM_INDEXABLE_RESOURCE_MIME_TYPES.insert("application/mspowerpoint");
  EDAM_INDEXABLE_RESOURCE_MIME_TYPES.insert("application/excel");
  EDAM_INDEXABLE_RESOURCE_MIME_TYPES.insert("application/vnd.ms-word");
  EDAM_INDEXABLE_RESOURCE_MIME_TYPES.insert("application/vnd.ms-powerpoint");
  EDAM_INDEXABLE_RESOURCE_MIME_TYPES.insert("application/vnd.ms-excel");
  EDAM_INDEXABLE_RESOURCE_MIME_TYPES.insert("application/vnd.openxmlformats-officedocument.wordprocessingml.document");
  EDAM_INDEXABLE_RESOURCE_MIME_TYPES.insert("application/vnd.openxmlformats-officedocument.presentationml.presentation");
  EDAM_INDEXABLE_RESOURCE_MIME_TYPES.insert("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");
  EDAM_INDEXABLE_RESOURCE_MIME_TYPES.insert("application/vnd.apple.pages");
  EDAM_INDEXABLE_RESOURCE_MIME_TYPES.insert("application/vnd.apple.numbers");
  EDAM_INDEXABLE_RESOURCE_MIME_TYPES.insert("application/vnd.apple.keynote");
  EDAM_INDEXABLE_RESOURCE_MIME_TYPES.insert("application/x-iwork-pages-sffpages");
  EDAM_INDEXABLE_RESOURCE_MIME_TYPES.insert("application/x-iwork-numbers-sffnumbers");
  EDAM_INDEXABLE_RESOURCE_MIME_TYPES.insert("application/x-iwork-keynote-sffkey");

  EDAM_INDEXABLE_PLAINTEXT_MIME_TYPES.insert("application/x-sh");
  EDAM_INDEXABLE_PLAINTEXT_MIME_TYPES.insert("application/x-bsh");
  EDAM_INDEXABLE_PLAINTEXT_MIME_TYPES.insert("application/sql");
  EDAM_INDEXABLE_PLAINTEXT_MIME_TYPES.insert("application/x-sql");

  EDAM_SEARCH_QUERY_LEN_MIN = 0;

  EDAM_SEARCH_QUERY_LEN_MAX = 1024;

  EDAM_SEARCH_QUERY_REGEX = "^[^\\p{Cc}\\p{Zl}\\p{Zp}]{0,1024}$";

  EDAM_HASH_LEN = 16;

  EDAM_USER_USERNAME_LEN_MIN = 1;

  EDAM_USER_USERNAME_LEN_MAX = 64;

  EDAM_USER_USERNAME_REGEX = "^[a-z0-9]([a-z0-9_-]{0,62}[a-z0-9])?$";

  EDAM_USER_NAME_LEN_MIN = 1;

  EDAM_USER_NAME_LEN_MAX = 255;

  EDAM_USER_NAME_REGEX = "^[^\\p{Cc}\\p{Zl}\\p{Zp}]{1,255}$";

  EDAM_TAG_NAME_LEN_MIN = 1;

  EDAM_TAG_NAME_LEN_MAX = 100;

  EDAM_TAG_NAME_REGEX = "^[^,\\p{Cc}\\p{Z}]([^,\\p{Cc}\\p{Zl}\\p{Zp}]{0,98}[^,\\p{Cc}\\p{Z}])?$";

  EDAM_NOTE_TITLE_LEN_MIN = 1;

  EDAM_NOTE_TITLE_LEN_MAX = 255;

  EDAM_NOTE_TITLE_REGEX = "^[^\\p{Cc}\\p{Z}]([^\\p{Cc}\\p{Zl}\\p{Zp}]{0,253}[^\\p{Cc}\\p{Z}])?$";

  EDAM_NOTE_CONTENT_LEN_MIN = 0;

  EDAM_NOTE_CONTENT_LEN_MAX = 5242880;

  EDAM_APPLICATIONDATA_NAME_LEN_MIN = 3;

  EDAM_APPLICATIONDATA_NAME_LEN_MAX = 32;

  EDAM_APPLICATIONDATA_VALUE_LEN_MIN = 0;

  EDAM_APPLICATIONDATA_VALUE_LEN_MAX = 4092;

  EDAM_APPLICATIONDATA_ENTRY_LEN_MAX = 4095;

  EDAM_APPLICATIONDATA_NAME_REGEX = "^[A-Za-z0-9_.-]{3,32}$";

  EDAM_APPLICATIONDATA_VALUE_REGEX = "^[\\p{Space}[^\\p{Cc}]]{0,4092}$";

  EDAM_NOTEBOOK_NAME_LEN_MIN = 1;

  EDAM_NOTEBOOK_NAME_LEN_MAX = 100;

  EDAM_NOTEBOOK_NAME_REGEX = "^[^\\p{Cc}\\p{Z}]([^\\p{Cc}\\p{Zl}\\p{Zp}]{0,98}[^\\p{Cc}\\p{Z}])?$";

  EDAM_NOTEBOOK_STACK_LEN_MIN = 1;

  EDAM_NOTEBOOK_STACK_LEN_MAX = 100;

  EDAM_NOTEBOOK_STACK_REGEX = "^[^\\p{Cc}\\p{Z}]([^\\p{Cc}\\p{Zl}\\p{Zp}]{0,98}[^\\p{Cc}\\p{Z}])?$";

  EDAM_WORKSPACE_NAME_LEN_MIN = 1;

  EDAM_WORKSPACE_NAME_LEN_MAX = 100;

  EDAM_WORKSPACE_DESCRIPTION_LEN_MAX = 600;

  EDAM_WORKSPACE_NAME_REGEX = "^[^\\p{Cc}\\p{Z}]([^\\p{Cc}\\p{Zl}\\p{Zp}]{0,98}[^\\p{Cc}\\p{Z}])?$";

  EDAM_PUBLISHING_URI_LEN_MIN = 1;

  EDAM_PUBLISHING_URI_LEN_MAX = 255;

  EDAM_PUBLISHING_URI_REGEX = "^[a-zA-Z0-9.~_+-]{1,255}$";

  EDAM_PUBLISHING_URI_PROHIBITED.insert(".");
  EDAM_PUBLISHING_URI_PROHIBITED.insert("..");

  EDAM_PUBLISHING_DESCRIPTION_LEN_MIN = 1;

  EDAM_PUBLISHING_DESCRIPTION_LEN_MAX = 200;

  EDAM_PUBLISHING_DESCRIPTION_REGEX = "^[^\\p{Cc}\\p{Z}]([^\\p{Cc}\\p{Zl}\\p{Zp}]{0,198}[^\\p{Cc}\\p{Z}])?$";

  EDAM_SAVED_SEARCH_NAME_LEN_MIN = 1;

  EDAM_SAVED_SEARCH_NAME_LEN_MAX = 100;

  EDAM_SAVED_SEARCH_NAME_REGEX = "^[^\\p{Cc}\\p{Z}]([^\\p{Cc}\\p{Zl}\\p{Zp}]{0,98}[^\\p{Cc}\\p{Z}])?$";

  EDAM_USER_PASSWORD_LEN_MIN = 6;

  EDAM_USER_PASSWORD_LEN_MAX = 64;

  EDAM_USER_PASSWORD_REGEX = "^[A-Za-z0-9!#$%&'()*+,./:;<=>?@^_`{|}~\\[\\]\\\\-]{6,64}$";

  EDAM_BUSINESS_URI_LEN_MAX = 32;

  EDAM_BUSINESS_MARKETING_CODE_REGEX_PATTERN = "[A-Za-z0-9-]{1,128}";

  EDAM_NOTE_TAGS_MAX = 100;

  EDAM_NOTE_RESOURCES_MAX = 1000;

  EDAM_USER_TAGS_MAX = 100000;

  EDAM_BUSINESS_TAGS_MAX = 100000;

  EDAM_USER_SAVED_SEARCHES_MAX = 100;

  EDAM_USER_NOTES_MAX = 100000;

  EDAM_BUSINESS_NOTES_MAX = 500000;

  EDAM_USER_NOTEBOOKS_MAX = 250;

  EDAM_USER_WORKSPACES_MAX = 0;

  EDAM_BUSINESS_NOTEBOOKS_MAX = 10000;

  EDAM_BUSINESS_WORKSPACES_MAX = 1000;

  EDAM_USER_RECENT_MAILED_ADDRESSES_MAX = 10;

  EDAM_USER_MAIL_LIMIT_DAILY_FREE = 50;

  EDAM_USER_MAIL_LIMIT_DAILY_PREMIUM = 200;

  EDAM_USER_UPLOAD_LIMIT_FREE = 62914560LL;

  EDAM_USER_UPLOAD_LIMIT_PREMIUM = 10737418240LL;

  EDAM_USER_UPLOAD_LIMIT_BUSINESS_FIRST_MONTH = 53687091200LL;

  EDAM_USER_UPLOAD_LIMIT_BUSINESS_NEXT_MONTH = 21474836480LL;

  EDAM_USER_UPLOAD_LIMIT_PLUS = 1073741824LL;

  EDAM_USER_UPLOAD_SURVEY_THRESHOLD = 5368709120LL;

  EDAM_USER_UPLOAD_LIMIT_BUSINESS = 10737418240LL;

  EDAM_USER_UPLOAD_LIMIT_BUSINESS_PER_USER = 2147483647LL;

  EDAM_NOTE_SIZE_MAX_FREE = 26214400;

  EDAM_NOTE_SIZE_MAX_PREMIUM = 209715200;

  EDAM_RESOURCE_SIZE_MAX_FREE = 26214400;

  EDAM_RESOURCE_SIZE_MAX_PREMIUM = 209715200;

  EDAM_USER_LINKED_NOTEBOOK_MAX = 100;

  EDAM_USER_LINKED_NOTEBOOK_MAX_PREMIUM = 500;

  EDAM_NOTEBOOK_BUSINESS_SHARED_NOTEBOOK_MAX = 5000;

  EDAM_NOTEBOOK_PERSONAL_SHARED_NOTEBOOK_MAX = 500;

  EDAM_NOTE_BUSINESS_SHARED_NOTE_MAX = 1000;

  EDAM_NOTE_PERSONAL_SHARED_NOTE_MAX = 100;

  EDAM_NOTE_CONTENT_CLASS_LEN_MIN = 3;

  EDAM_NOTE_CONTENT_CLASS_LEN_MAX = 32;

  EDAM_NOTE_CONTENT_CLASS_REGEX = "^[A-Za-z0-9_.-]{3,32}$";

  EDAM_HELLO_APP_CONTENT_CLASS_PREFIX = "evernote.hello.";

  EDAM_FOOD_APP_CONTENT_CLASS_PREFIX = "evernote.food.";

  EDAM_CONTENT_CLASS_HELLO_ENCOUNTER = "evernote.hello.encounter";

  EDAM_CONTENT_CLASS_HELLO_PROFILE = "evernote.hello.profile";

  EDAM_CONTENT_CLASS_FOOD_MEAL = "evernote.food.meal";

  EDAM_CONTENT_CLASS_SKITCH_PREFIX = "evernote.skitch";

  EDAM_CONTENT_CLASS_SKITCH = "evernote.skitch";

  EDAM_CONTENT_CLASS_SKITCH_PDF = "evernote.skitch.pdf";

  EDAM_CONTENT_CLASS_PENULTIMATE_PREFIX = "evernote.penultimate.";

  EDAM_CONTENT_CLASS_PENULTIMATE_NOTEBOOK = "evernote.penultimate.notebook";

  EDAM_SOURCE_APPLICATION_POSTIT = "postit";

  EDAM_SOURCE_APPLICATION_MOLESKINE = "moleskine";

  EDAM_SOURCE_APPLICATION_EN_SCANSNAP = "scanner.scansnap.evernote";

  EDAM_SOURCE_APPLICATION_EWC = "clipncite.web";

  EDAM_SOURCE_APPLICATION_ANDROID_SHARE_EXTENSION = "android.clipper.evernote";

  EDAM_SOURCE_APPLICATION_IOS_SHARE_EXTENSION = "ios.clipper.evernote";

  EDAM_SOURCE_APPLICATION_WEB_CLIPPER = "webclipper.evernote";

  EDAM_SOURCE_OUTLOOK_CLIPPER = "app.ms.outlook";

  EDAM_NOTE_TITLE_QUALITY_UNTITLED = 0;

  EDAM_NOTE_TITLE_QUALITY_LOW = 1;

  EDAM_NOTE_TITLE_QUALITY_MEDIUM = 2;

  EDAM_NOTE_TITLE_QUALITY_HIGH = 3;

  EDAM_RELATED_PLAINTEXT_LEN_MIN = 1;

  EDAM_RELATED_PLAINTEXT_LEN_MAX = 131072;

  EDAM_RELATED_MAX_NOTES = 25;

  EDAM_RELATED_MAX_NOTEBOOKS = 1;

  EDAM_RELATED_MAX_TAGS = 25;

  EDAM_RELATED_MAX_EXPERTS = 10;

  EDAM_RELATED_MAX_RELATED_CONTENT = 10;

  EDAM_BUSINESS_NOTEBOOK_DESCRIPTION_LEN_MIN = 1;

  EDAM_BUSINESS_NOTEBOOK_DESCRIPTION_LEN_MAX = 200;

  EDAM_BUSINESS_NOTEBOOK_DESCRIPTION_REGEX = "^[^\\p{Cc}\\p{Z}]([^\\p{Cc}\\p{Zl}\\p{Zp}]{0,198}[^\\p{Cc}\\p{Z}])?$";

  EDAM_BUSINESS_PHONE_NUMBER_LEN_MAX = 20;

  EDAM_PREFERENCE_NAME_LEN_MIN = 3;

  EDAM_PREFERENCE_NAME_LEN_MAX = 32;

  EDAM_PREFERENCE_VALUE_LEN_MIN = 1;

  EDAM_PREFERENCE_VALUE_LEN_MAX = 1024;

  EDAM_MAX_PREFERENCES = 100;

  EDAM_MAX_VALUES_PER_PREFERENCE = 256;

  EDAM_PREFERENCE_ONLY_ONE_VALUE_LEN_MAX = 16384;

  EDAM_PREFERENCE_NAME_REGEX = "^[A-Za-z0-9_.-]{3,32}$";

  EDAM_PREFERENCE_VALUE_REGEX = "^[^\\p{Cc}]{1,1024}$";

  EDAM_PREFERENCE_ONLY_ONE_VALUE_REGEX = "^[^\\p{Cc}]{1,16384}$";

  EDAM_PREFERENCE_SHORTCUTS = "evernote.shortcuts";

  EDAM_PREFERENCE_BUSINESS_DEFAULT_NOTEBOOK = "evernote.business.notebook";

  EDAM_PREFERENCE_BUSINESS_QUICKNOTE = "evernote.business.quicknote";

  EDAM_PREFERENCE_SHORTCUTS_MAX_VALUES = 250;

  EDAM_DEVICE_ID_LEN_MAX = 32;

  EDAM_DEVICE_ID_REGEX = "^[^\\p{Cc}]{1,32}$";

  EDAM_DEVICE_DESCRIPTION_LEN_MAX = 64;

  EDAM_DEVICE_DESCRIPTION_REGEX = "^[^\\p{Cc}]{1,64}$";

  EDAM_SEARCH_SUGGESTIONS_MAX = 10;

  EDAM_SEARCH_SUGGESTIONS_PREFIX_LEN_MAX = 1024;

  EDAM_SEARCH_SUGGESTIONS_PREFIX_LEN_MIN = 2;

  EDAM_FIND_CONTACT_DEFAULT_MAX_RESULTS = 100;

  EDAM_FIND_CONTACT_MAX_RESULTS = 256;

  EDAM_NOTE_LOCK_VIEWERS_NOTES_MAX = 150;

  EDAM_GET_ORDERS_MAX_RESULTS = 2000;

  EDAM_MESSAGE_BODY_LEN_MAX = 2048;

  EDAM_MESSAGE_BODY_REGEX = "^[^\\p{Cc}\\p{Z}]([^\\p{Cc}\\p{Zl}\\p{Zp}]{0,2046}[^\\p{Cc}\\p{Z}])?$";

  EDAM_MESSAGE_RECIPIENTS_MAX = 50;

  EDAM_MESSAGE_ATTACHMENTS_MAX = 100;

  EDAM_MESSAGE_ATTACHMENT_TITLE_LEN_MAX = 255;

  EDAM_MESSAGE_ATTACHMENT_TITLE_REGEX = "^[^\\p{Cc}\\p{Z}]([^\\p{Cc}\\p{Zl}\\p{Zp}]{0,253}[^\\p{Cc}\\p{Z}])?$";

  EDAM_MESSAGE_ATTACHMENT_SNIPPET_LEN_MAX = 2048;

  EDAM_MESSAGE_ATTACHMENT_SNIPPET_REGEX = "^[^\\p{Cc}\\p{Z}]([\\n[^\\p{Cc}\\p{Zl}\\p{Zp}]]{0,2046}[^\\p{Cc}\\p{Z}])?$";

  EDAM_USER_PROFILE_PHOTO_MAX_BYTES = 716800;

  EDAM_PROMOTION_ID_LEN_MAX = 32;

  EDAM_PROMOTION_ID_REGEX = "^[A-Za-z0-9_.-]{1,32}$";

  EDAM_APP_RATING_MIN = 1;

  EDAM_APP_RATING_MAX = 5;

  EDAM_SNIPPETS_NOTES_MAX = 24;

  EDAM_CONNECTED_IDENTITY_REQUEST_MAX = 100;

  EDAM_OPEN_ID_ACCESS_TOKEN_MAX = 1000;

}

}} // namespace

